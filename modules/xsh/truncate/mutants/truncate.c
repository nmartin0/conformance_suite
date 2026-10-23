/*
 * truncate.c - planted faults in truncate(), for mutation checks (CI only).
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

int truncate(const char *path, off_t length)
{
	REAL(truncate);
	const char *m = mutant_of("truncate.");
	struct stat st;
	struct timespec ts[2];
	int r;

	if (m == NULL)
		return real_truncate(path, length);
	if (IS("noop") || (IS("missing_ok") && stat(path, &st) != 0))
		return 0;
	if (IS("off_by_one") && length > 0)
		length++;
	if (IS("negative_ok") && length < 0)
		return 0;
	if (IS("special_ok") && stat(path, &st) == 0 && (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode)))
		return 0;
	if (IS("limit_off_by_one")) {
		struct rlimit rl;
		if (getrlimit(RLIMIT_FSIZE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY && (rlim_t)length >= rl.rlim_cur)
			return fail(EFBIG);	/* refuses a length equal to the limit */
	}
	if (IS("limit_ignored") || IS("no_signal")) {
		struct rlimit rl, save;
		if (getrlimit(RLIMIT_FSIZE, &save) == 0 && save.rlim_cur != RLIM_INFINITY && (rlim_t)length > save.rlim_cur) {
			if (IS("no_signal"))
				return fail(EFBIG);	/* refuses, but raises no signal */
			rl = save;
			rl.rlim_cur = save.rlim_max;
			(void)setrlimit(RLIMIT_FSIZE, &rl);
			r = real_truncate(path, length);
			(void)setrlimit(RLIMIT_FSIZE, &save);
			return r;
		}
	}
	if (IS("permission_ignored") && geteuid() != 0 && stat(path, &st) == 0 && st.st_uid == geteuid() &&
	    !(st.st_mode & S_IWUSR)) {
		mode_t keep = st.st_mode & 07777;
		(void)chmod(path, keep | S_IWUSR);
		r = real_truncate(path, length);
		(void)chmod(path, keep);
		return r;
	}
	if (IS("times_kept") && stat(path, &st) == 0) {
		r = real_truncate(path, length);
		ts[0] = st.st_atim;
		ts[1] = st.st_mtim;
		(void)utimensat(AT_FDCWD, path, ts, 0);
		return r;
	}
	if (IS("moves_offsets") && stat(path, &st) == 0) {
		struct stat fs;
		int fd;

		r = real_truncate(path, length);
		for (fd = 0; fd < 256; fd++)
			if (fstat(fd, &fs) == 0 && fs.st_dev == st.st_dev && fs.st_ino == st.st_ino)
				(void)lseek(fd, 0, SEEK_SET);
		return r;
	}
	if (IS("extend_garbage") && stat(path, &st) == 0 && length > st.st_size) {
		r = real_truncate(path, length);
		if (r == 0) {
			int fd = open(path, O_WRONLY);
			if (fd >= 0) {
				(void)pwrite(fd, "G", 1, length - 1);
				(void)close(fd);
			}
		}
		return r;
	}
	r = real_truncate(path, length);
	if (r == -1 && IS("eisdir_as_einval") && errno == EISDIR)
		errno = EINVAL;
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
