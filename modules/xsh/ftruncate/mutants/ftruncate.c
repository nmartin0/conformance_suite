/*
 * ftruncate.c - planted faults in ftruncate(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#include <sys/resource.h>
#include <time.h>
static int fail(int e)
{
	errno = e;
	return -1;
}

int ftruncate(int fd, off_t length)
{
	REAL(ftruncate);
	const char *m = mutant_of("ftruncate.");
	struct stat st;
	struct timespec ts[2];
	int r;

	if (m == NULL)
		return real_ftruncate(fd, length);
	if (IS("noop"))
		return 0;
	if (IS("off_by_one") && length > 0)
		length++;
	if (IS("negative_ok") && length < 0)
		return 0;
	if (IS("bad_ok") && fcntl(fd, F_GETFD) == -1)
		return 0;
	if (IS("readonly_ok") && (fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDONLY)
		return 0;
	if (IS("pipe_ok") && fstat(fd, &st) == 0 && (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode)))
		return 0;
	if (IS("moves_offset")) {
		r = real_ftruncate(fd, length);
		(void)lseek(fd, 0, SEEK_SET);
		return r;
	}
	if (IS("limit_ignored") || IS("no_signal")) {
		struct rlimit rl, save;
		if (getrlimit(RLIMIT_FSIZE, &save) == 0 && save.rlim_cur != RLIM_INFINITY && (rlim_t)length > save.rlim_cur) {
			if (IS("no_signal"))
				return fail(EFBIG);
			rl = save;
			rl.rlim_cur = RLIM_INFINITY;
			(void)setrlimit(RLIMIT_FSIZE, &rl);
			r = real_ftruncate(fd, length);
			(void)setrlimit(RLIMIT_FSIZE, &save);
			return r;
		}
	}
	if (IS("times_kept") && fstat(fd, &st) == 0) {
		r = real_ftruncate(fd, length);
		ts[0] = st.st_atim;
		ts[1] = st.st_mtim;
		(void)futimens(fd, ts);
		return r;
	}
	if (IS("extend_garbage") && fstat(fd, &st) == 0 && length > st.st_size) {
		r = real_ftruncate(fd, length);
		if (r == 0)
			(void)pwrite(fd, "G", 1, length - 1);
		return r;
	}
	r = real_ftruncate(fd, length);
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
