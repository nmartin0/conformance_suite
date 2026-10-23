/*
 * scandir.c - planted faults in scandir(), for mutation checks (CI only).
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

static int (*sel_saved)(const struct dirent *);

/* Calls the selector twice for each entry. */
static int sel_both(const struct dirent *e)
{
	(void)sel_saved(e);
	return sel_saved(e);
}

int scandir(const char *dir, struct dirent ***nl, int (*sel)(const struct dirent *),
	    int (*compar)(const struct dirent **, const struct dirent **))
{
	REAL(scandir);
	const char *m = mutant_of("scandir.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_scandir(dir, nl, sel, compar);
	if (IS("fails"))
		return fail(EIO);
	if (IS("missing_ok") && stat(dir, &st) != 0) {
		*nl = malloc(sizeof **nl);
		return *nl != NULL ? 0 : -1;
	}
	if (IS("ignores_sel"))
		sel = NULL;
	if (IS("null_sel_none") && sel == NULL) {
		*nl = malloc(sizeof **nl);
		return *nl != NULL ? 0 : -1;
	}
	if (IS("sel_twice") && sel != NULL) {
		sel_saved = sel;
		sel = sel_both;
	}
	if (IS("always_alphasort"))
		compar = alphasort;
	r = real_scandir(dir, nl, sel, compar);
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	if (r > 1 && IS("unsorted")) {
		/* sorted, then the first two swapped: surely not in order */
		struct dirent *x = (*nl)[0];
		(*nl)[0] = (*nl)[1];
		(*nl)[1] = x;
	}
	return r;
}
