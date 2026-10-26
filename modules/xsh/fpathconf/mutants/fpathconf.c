/*
 * fpathconf.c - planted faults in pathconf() and fpathconf(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "fpathconf.<fault>" and apply only
 * while PC_PHASE is "test" (followed through pc.c's setenv() calls).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

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

static long fail(int e)
{
	errno = e;
	return -1;
}

/*
 * The fault, given the real answer r (errno e0 before the call, e after)
 * and the file's type (st valid when have_st).
 */
static long fault(const char *m, int name, long r, int e0, int have_st, const struct stat *st)
{
	if (IS("dir_einval_link_max") && name == _PC_LINK_MAX && have_st && S_ISDIR(st->st_mode))
		return fail(EINVAL);
	if (IS("reg_einval_chown") && name == _PC_CHOWN_RESTRICTED && have_st && S_ISREG(st->st_mode))
		return fail(EINVAL);
	if (IS("nolimit_sets_errno") && r == -1 && errno == e0)
		return fail(ENOSYS);
	if (IS("success_sets_errno") && r != -1) {
		errno = ENOENT;	/* a stale errno from a probe of the file */
		return r;
	}
	if (IS("pipe_buf_256") && name == _PC_PIPE_BUF)
		return 256;
	if (IS("pipe_einval") && name == _PC_PIPE_BUF && have_st && S_ISFIFO(st->st_mode))
		return fail(EINVAL);
	if (IS("path_max_255") && name == _PC_PATH_MAX)
		return 255;	/* {_POSIX_PATH_MAX} less the terminating null */
	if (IS("name_max_14") && name == _PC_NAME_MAX)
		return 14;
	if (IS("no_trunc_off") && name == _PC_NO_TRUNC)
		return -1;
#ifdef _PC_TIMESTAMP_RESOLUTION
	if (IS("resolution_zero") && name == _PC_TIMESTAMP_RESOLUTION)
		return 0;
#endif
	if (IS("max_canon_small") && name == _PC_MAX_CANON)
		return 127;
	if (IS("tty_einval") && name == _PC_MAX_INPUT && have_st && S_ISCHR(st->st_mode))
		return fail(EINVAL);
	if (IS("name_max_plus_one") && name == _PC_NAME_MAX && r > 0)
		return r + 1;
	if ((IS("invalid_returns_zero") || IS("invalid_wrong_errno")) && r == -1 && errno == EINVAL && !have_st)
		return IS("invalid_returns_zero") ? 0 : fail(ENOSYS);
	return r;
}

/* Names this library knows to be valid (so EINVAL for others means "invalid name"). */
static int known(int name)
{
	switch (name) {
	case _PC_LINK_MAX: case _PC_MAX_CANON: case _PC_MAX_INPUT: case _PC_NAME_MAX: case _PC_PATH_MAX:
	case _PC_PIPE_BUF: case _PC_CHOWN_RESTRICTED: case _PC_NO_TRUNC: case _PC_VDISABLE:
		return 1;
	}
	return 0;
}

long pathconf(const char *path, int name)
{
	static long (*real)(const char *, int);
	const char *m = mutant_of("fpathconf.");
	struct stat st;
	long r;
	int e0 = errno, have;

	if (real == NULL)
		real = (long (*)(const char *, int))dlsym(RTLD_NEXT, "pathconf");
	if (m == NULL)
		return real(path, name);
	if (IS("missing_path_enosys") && stat(path, &st) != 0 && errno == ENOENT)
		return fail(ENOSYS);	/* an error number the text does not allow here */
	have = stat(path, &st) == 0;
	errno = e0;
	r = real(path, name);
	if (IS("invalid_returns_zero") || IS("invalid_wrong_errno"))
		have = have && known(name);
	return fault(m, name, r, e0, have, &st);
}

long fpathconf(int fd, int name)
{
	static long (*real)(int, int);
	const char *m = mutant_of("fpathconf.");
	struct stat st;
	long r;
	int e0 = errno, have;

	if (real == NULL)
		real = (long (*)(int, int))dlsym(RTLD_NEXT, "fpathconf");
	if (m == NULL)
		return real(fd, name);
	if (IS("bad_fd_enosys") && fcntl(fd, F_GETFD) == -1)
		return fail(ENOSYS);	/* an error number the text does not allow here */
	have = fstat(fd, &st) == 0;
	errno = e0;
	r = real(fd, name);
	if (IS("invalid_returns_zero") || IS("invalid_wrong_errno"))
		have = have && known(name);
	return fault(m, name, r, e0, have, &st);
}

/* "truncates_long_names": open() cuts a final component longer than {NAME_MAX}. */
int open(const char *path, int flags, ...)
{
	static int (*real)(const char *, int, ...);
	const char *m = mutant_of("fpathconf.");
	char cut[4096];
	mode_t mode = 0;
	va_list ap;

	if (real == NULL)
		real = (int (*)(const char *, int, ...))dlsym(RTLD_NEXT, "open");
	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	if (IS("truncates_long_names") && strchr(path, '/') == NULL && strlen(path) > 255 && strlen(path) < sizeof cut) {
		memcpy(cut, path, 255);
		cut[255] = '\0';
		return real(cut, flags, mode);
	}
	return real(path, flags, mode);
}
