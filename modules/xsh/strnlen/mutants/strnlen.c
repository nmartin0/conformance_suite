/*
 * strnlen.c - planted faults in strlen() and strnlen(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strnlen.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * Nothing here calls strlen() or strnlen(), which it replaces.
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

static int my_prefix(const char *s, const char *p)
{
	while (*p != '\0' && *s == *p)
		s++, p++;
	return *p == '\0';
}

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);
	size_t n;

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	n = my_len(value);
	if (my_eq(name, "PC_PHASE") && n < sizeof phase) {
		size_t i;

		for (i = 0; i <= n; i++)
			phase[i] = value[i];
	}
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");

	if (!my_eq(phase, "test") || m == NULL || !my_prefix(m, prefix))
		return NULL;
	return m + my_len(prefix);
}

#define IS(name) (m != NULL && my_eq(m, name))
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

size_t strlen(const char *s)
{
	REAL(strlen);
	const char *m = mutant_of("strnlen.");
	size_t n;

	if (m == NULL)
		return real_strlen(s);
	if (IS("strlen_counts_nul"))
		return my_len(s) + 1;
	if (IS("strlen_stops_high")) {
		/* Compares plain (signed) chars with > 0: stops at 0x80 and above. */
		const signed char *p = (const signed char *)s;

		for (n = 0; p[n] > 0; n++)
			;
		return n;
	}
	n = real_strlen(s);
	if (IS("strlen_errno_clobbered"))
		errno = EINVAL;
	return n;
}

size_t strnlen(const char *s, size_t max)
{
	REAL(strnlen);
	const char *m = mutant_of("strnlen.");
	size_t n;

	if (m == NULL)
		return real_strnlen(s, max);
	if (IS("ignores_max") || IS("uses_strlen")) {
		n = my_len(s);
		return IS("uses_strlen") && n > max ? max : n;
	}
	if (IS("off_by_one")) {
		n = real_strnlen(s, max);
		return n == max && n > 0 ? n - 1 : n;
	}
	if (IS("reads_extra")) {
		/* Checks s[max] before deciding (one byte too far). */
		for (n = 0; n <= max && s[n] != '\0'; n++)
			;
		return n > max ? max : n;
	}
	if (IS("zero_reads")) {
		volatile char c = s[0];	/* looks at the first byte even for max 0 */

		(void)c;
	}
	if (IS("counts_nul")) {
		/* Counts the null byte when it finds one. */
		n = real_strnlen(s, max);
		return n < max ? n + 1 : n;
	}
	n = real_strnlen(s, max);
	if (IS("errno_clobbered"))
		errno = EINVAL;
	return n;
}
