/*
 * readdir_r.c - planted faults in readdir_r(), for mutation checks (CI only).
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

#include <stddef.h>

int readdir_r(DIR *d, struct dirent *entry, struct dirent **result)
{
	REAL(readdir_r);
	const char *m = mutant_of("readdir_r.");
	static union { struct dirent d; char pad[sizeof(struct dirent) + 512]; } own;
	int r;

	if (m == NULL)
		return real_readdir_r(d, entry, result);
	r = real_readdir_r(d, entry, result);
	if (r == 0 && *result != NULL && IS("other_entry")) {
		memcpy(&own.d, entry, sizeof own.d);
		snprintf(own.d.d_name, 256, "%s", entry->d_name);
		*result = &own.d;
	}
	if (r == 0 && *result == NULL && IS("no_end")) {
		r = ENOENT;
	}
	if (r == 0 && *result != NULL && IS("overruns"))
		memset((char *)entry + offsetof(struct dirent, d_name) + strlen(entry->d_name) + 1, 0, 16);
	return r;
}
