/*
 * fdopen.c - planted faults in fdopen(), for mutation checks (CI only).
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

#include <stdio.h>

FILE *fdopen(int fd, const char *mode)
{
	REAL(fdopen);
	const char *m = mutant_of("fdopen.");
	char base[8];
	FILE *f;
	int d, fl;

	if (m == NULL)
		return real_fdopen(fd, mode);
	if (IS("x_rejected") && strchr(mode, 'x')) {
		errno = EINVAL;
		return NULL;
	}
	if (IS("b_rejected") && strchr(mode, 'b')) {
		errno = EINVAL;
		return NULL;
	}
	if (IS("a_seeks_end") && mode[0] == 'a')
		(void)lseek(fd, 0, SEEK_END);
	if (IS("dups") && (d = dup(fd)) != -1)
		fd = d;
	if (IS("rewinds"))
		(void)lseek(fd, 0, SEEK_SET);
	if (IS("truncates") && mode[0] == 'w')
		(void)ftruncate(fd, 0);
	if (IS("read_only") && strchr(mode, '+'))
		snprintf(base, sizeof base, "%c", mode[0]);
	else
		snprintf(base, sizeof base, "%s", mode);
	if (IS("cloexec_changed") && !strchr(mode, 'e') && (fl = fcntl(fd, F_GETFD)) != -1)
		(void)fcntl(fd, F_SETFD, fl ^ FD_CLOEXEC);
	f = real_fdopen(fd, base);
	if (f == NULL) {
		if (IS("wrong_errno"))
			errno = errno == EIO ? EINVAL : EIO;
		return NULL;
	}
	if (IS("clears_append") && (fl = fcntl(fd, F_GETFL)) != -1)
		(void)fcntl(fd, F_SETFL, fl & ~O_APPEND);
	if (IS("e_ignored") && strchr(mode, 'e') && (fl = fcntl(fd, F_GETFD)) != -1)
		(void)fcntl(fd, F_SETFD, fl & ~FD_CLOEXEC);
	if (IS("error_set")) {
		if (mode[0] == 'r' && !strchr(mode, '+'))
			(void)fputc('x', f);
		else
			(void)fgetc(f);
	}
	return f;
}
