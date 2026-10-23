/*
 * opendir.c - planted faults in opendir(), for mutation checks (CI only).
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

static int fail(int e)
{
	errno = e;
	return -1;
}

static void *fail_p(int e)
{
	errno = e;
	return NULL;
}

DIR *opendir(const char *name)
{
	REAL(opendir);
	const char *m = mutant_of("opendir.");
	struct stat st;
	DIR *d;

	if (m == NULL)
		return real_opendir(name);
	if (IS("fails"))
		return fail_p(EIO);
	if (IS("read_ignored") && stat(name, &st) == 0 && S_ISDIR(st.st_mode) && st.st_uid == geteuid() &&
	    !(st.st_mode & S_IRUSR)) {
		mode_t keep = st.st_mode & 07777;
		(void)chmod(name, keep | S_IRUSR);
		d = real_opendir(name);
		(void)chmod(name, keep);
		return d;
	}
	d = real_opendir(name);
	if (d != NULL && IS("no_cloexec")) {
		int fd = dirfd(d);
		if (fd >= 0)
			(void)fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) & ~FD_CLOEXEC);
	}
	if (d == NULL && IS("missing_as_enotdir") && errno == ENOENT)
		errno = ENOTDIR;
	if (d == NULL && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (d == NULL && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (d == NULL && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (d == NULL && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	return d;
}
