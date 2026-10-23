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

/*
 * rewinddir.stale: the stream keeps showing the entries it had when it was
 * opened. opendir() takes a snapshot of the names; after rewinddir(),
 * readdir() replays it.
 */
#define SNAP 64
static DIR *snap_dir;
static int snap_n, snap_pos = -1;
static union { struct dirent d; char pad[sizeof(struct dirent) + 256]; } snap[SNAP];

DIR *opendir(const char *name)
{
	REAL(opendir);
	REAL(readdir);
	REAL(closedir);
	const char *m = mutant_of("rewinddir.");
	DIR *d = real_opendir(name), *s;
	struct dirent *e;

	if (d != NULL && IS("stale") && (s = real_opendir(name)) != NULL) {
		snap_dir = d;
		snap_n = 0;
		snap_pos = -1;
		while (snap_n < SNAP && (e = real_readdir(s)) != NULL) {
			memcpy(&snap[snap_n].d, e, sizeof snap[snap_n].d);
			snprintf(snap[snap_n].d.d_name, 256, "%s", e->d_name);
			snap_n++;
		}
		(void)real_closedir(s);
	}
	return d;
}

struct dirent *readdir(DIR *d)
{
	REAL(readdir);

	if (d == snap_dir && snap_pos >= 0)
		return snap_pos < snap_n ? &snap[snap_pos++].d : NULL;
	return real_readdir(d);
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
	real_rewinddir(d);
	if (IS("stale") && d == snap_dir)
		snap_pos = 0;
}
