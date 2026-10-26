/*
 * stpcpy.c - planted faults in stpcpy() and strcpy(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "stpcpy.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * Nothing here calls strcpy() or stpcpy(), which it replaces.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char phase[16];

static size_t my_len(const char *s)
{
	size_t n = 0;

	while (s[n] != '\0')
		n++;
	return n;
}

static int my_eq(const char *a, const char *b)
{
	while (*a != '\0' && *a == *b)
		a++, b++;
	return *a == *b;
}

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (my_eq(name, "PC_PHASE") && my_len(value) < sizeof phase)
		memcpy(phase, value, my_len(value) + 1);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = my_len(prefix);

	if (!my_eq(phase, "test") || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && my_eq(m, name))
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

/* The faulty copy: returns the address of the copied null byte. */
static char *copy(char *d, const char *s, int nul, int extra)
{
	size_t n = my_len(s), i;

	for (i = 0; i < n; i++)
		d[i] = s[i];
	if (nul)
		d[n] = '\0';
	if (extra)
		d[n + 1] = '\0';	/* stores a whole word, one byte too many */
	return d + n;
}

char *stpcpy(char *restrict d, const char *restrict s)
{
	REAL(stpcpy);
	const char *m = mutant_of("stpcpy.");
	char *r;

	if (m == NULL)
		return real_stpcpy(d, s);
	if (IS("returns_s1")) {
		(void)copy(d, s, 1, 0);
		return d;
	}
	if (IS("past_nul"))
		return copy(d, s, 1, 0) + 1;
	if (IS("no_nul"))
		return copy(d, s, 0, 0);
	if (IS("writes_past"))
		return copy(d, s, 1, 1);
	r = real_stpcpy(d, s);
	if (IS("errno_clobbered"))
		errno = EINVAL;
	return r;
}

char *strcpy(char *restrict d, const char *restrict s)
{
	REAL(strcpy);
	const char *m = mutant_of("stpcpy.");
	char *r;

	if (m == NULL)
		return real_strcpy(d, s);
	if (IS("strcpy_returns_end"))
		return copy(d, s, 1, 0);
	if (IS("strcpy_no_nul")) {
		(void)copy(d, s, 0, 0);
		return d;
	}
	if (IS("strcpy_writes_past")) {
		(void)copy(d, s, 1, 1);
		return d;
	}
	r = real_strcpy(d, s);
	if (IS("strcpy_errno_clobbered"))
		errno = ENOENT;
	return r;
}
