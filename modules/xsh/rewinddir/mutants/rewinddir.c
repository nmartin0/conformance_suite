/*
 * rewinddir.c - planted faults in rewinddir(), for mutation checks (CI only).
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

void rewinddir(DIR *d)
{
	REAL(rewinddir);
	const char *m = mutant_of("rewinddir.");

	if (m == NULL) {
		real_rewinddir(d);
		return;
	}
	if (IS("noop"))
		return;
	if (IS("stale")) {
		/* restarts, but keeps a removed name in view: re-create d/b while reading */
		real_rewinddir(d);
		{
			int fd = dirfd(d), b;
			if (fd >= 0 && (b = openat(fd, "b", O_WRONLY | O_CREAT | O_EXCL, 0644)) >= 0)
				(void)close(b);
		}
		return;
	}
	real_rewinddir(d);
}
