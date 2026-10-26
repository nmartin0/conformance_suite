/*
 * memccpy.c - planted faults in memccpy(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "memccpy.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
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

void *memccpy(void *restrict d, const void *restrict s, int c, size_t n)
{
	REAL(memccpy);
	const char *m = mutant_of("memccpy.");
	unsigned char *dp = d;
	const unsigned char *sp = s;
	size_t i;
	void *r;

	if (m == NULL)
		return real_memccpy(d, s, c, n);
	if (IS("returns_at_c") || IS("no_stop") || IS("stops_before") || IS("not_null") ||
	    IS("copies_extra") || IS("int_compare")) {
		for (i = 0; i < n; i++) {
			int hit = IS("int_compare") ? ((int)sp[i] == c) : (sp[i] == (unsigned char)c);	/* no conversion of c */

			if (hit && IS("stops_before"))
				return dp + i;
			dp[i] = sp[i];
			if (hit && !IS("no_stop"))
				return IS("returns_at_c") ? dp + i : dp + i + 1;
		}
		if (IS("copies_extra") && n > 0)
			dp[n] = sp[n - 1];	/* one byte too many */
		return IS("not_null") ? dp + n : NULL;
	}
	if (IS("reads_extra")) {
		/* Prefetches one byte past the n-byte area. */
		volatile unsigned char x = sp[n];

		(void)x;
	}
	r = real_memccpy(d, s, c, n);
	if (IS("errno_clobbered"))
		errno = EINVAL;
	if (IS("errno_not_found") && r == NULL)
		errno = ENOENT;
	return r;
}
