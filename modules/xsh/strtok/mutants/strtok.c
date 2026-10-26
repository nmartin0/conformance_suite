/*
 * strtok.c - planted faults in strtok() and strtok_r(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strtok.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * A faulty call runs the small tokenizer below, with the fault switched
 * on; the others go to the C library.
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

static int in(char c, const char *sep)
{
	for (; *sep != '\0'; sep++)
		if (*sep == c)
			return 1;
	return 0;
}

/* The tokenizer, with the faults that m names. */
static char *tok(const char *m, char *s, const char *sep, char **save)
{
	char *start, *p, *token;

	if (s == NULL)
		s = *save;
	if (s == NULL)
		return NULL;
	if (IS("empty_sep_null") && *sep == '\0')
		return NULL;
	start = s;
	if (!IS("skips_no_leading") && !IS("r_skips_no_leading"))
		while (*s != '\0' && in(*s, sep))
			s++;
	if (IS("nul_leading"))
		for (p = start; p < s; p++)
			*p = '\0';
	if (*s == '\0' && !IS("skips_no_leading") && !IS("r_skips_no_leading")) {
		*save = s;
		return IS("no_null_at_end") ? s : NULL;
	}
	token = s;
	while (*s != '\0' && !in(*s, sep))
		s++;
	if (*s != '\0') {
		*s++ = '\0';
		if (IS("nul_all_seps"))
			while (*s != '\0' && in(*s, sep))
				*s++ = '\0';
	}
	*save = s;
	return token;
}

static char *last;	/* strtok()'s position under a fault */

char *strtok(char *restrict s, const char *restrict sep)
{
	REAL(strtok);
	const char *m = mutant_of("strtok.");
	static const char *first;
	char *r;

	if (m == NULL)
		return real_strtok(s, sep);
	if (IS("fixed_sep")) {
		if (s != NULL)
			first = sep;	/* remembers the first call's separators */
		return tok(m, s, first, &last);
	}
	if (IS("skips_no_leading") || IS("no_null_at_end") || IS("nul_all_seps") ||
	    IS("nul_leading") || IS("empty_sep_null") || IS("r_clobbers_strtok"))
		return tok(m, s, sep, &last);
	r = real_strtok(s, sep);
	if (IS("errno_clobbered") && r == NULL)
		errno = ESRCH;	/* "no token" reported through errno */
	return r;
}

char *strtok_r(char *restrict s, const char *restrict sep, char **restrict state)
{
	REAL(strtok_r);
	const char *m = mutant_of("strtok.");
	static char *mine;
	char *r;

	if (m == NULL)
		return real_strtok_r(s, sep, state);
	if (IS("r_uses_static"))
		return tok(m, s, sep, &mine);	/* one position for every caller */
	if (IS("r_skips_no_leading"))
		return tok(m, s, sep, state);
	if (IS("r_clobbers_strtok")) {
		r = tok(m, s, sep, state);
		last = *state;	/* built on strtok()'s own position */
		return r;
	}
	if (IS("r_uses_state_first") && s != NULL && *state != NULL)
		return tok(m, NULL, sep, state);
	r = real_strtok_r(s, sep, state);
	if (IS("r_errno_clobbered"))
		errno = ENOENT;
	return r;
}
