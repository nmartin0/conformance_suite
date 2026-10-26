/*
 * fchdir.c - planted faults in fchdir() (and open(), for the by_name fault), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "fchdir.<fault>" and apply only
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


#include <stdarg.h>

/* "by_name": the name each descriptor was opened with (all phases). */
static char names[64][256];

int open(const char *path, int flags, ...)
{
	const char *m = getenv("PC_MUTANT");
	mode_t mode = 0;
	int fd;
	REAL(open);

	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	fd = real_open(path, flags, mode);
	if (fd >= 0 && fd < 64 && m != NULL && strcmp(m, "fchdir.by_name") == 0 && strlen(path) < sizeof names[0])
		strcpy(names[fd], path);
	return fd;
}

int fchdir(int fd)
{
	const char *m = mutant_of("fchdir.");
	int r, e;
	REAL(fchdir);

	if (m == NULL)
		return real_fchdir(fd);
	if (IS("noop"))
		return 0;
	if (IS("by_name") && fd >= 0 && fd < 64 && names[fd][0] != '\0')
		return chdir(names[fd]);	/* goes back by name, as getcwd() and chdir() would */
	r = real_fchdir(fd);
	e = errno;
	if (r == 0 && IS("returns_one"))
		return 1;
	if (r == 0 && IS("closes_fd"))
		(void)close(fd);
	if (r == -1 && IS("file_ok") && e == ENOTDIR)
		return 0;
	if (r == -1 && IS("ebadf_as_enotdir") && e == EBADF)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_ebadf") && e == ENOTDIR)
		errno = EBADF;
	if (r == -1 && IS("eacces_as_eperm") && e == EACCES)
		errno = EPERM;
	return r;
}
