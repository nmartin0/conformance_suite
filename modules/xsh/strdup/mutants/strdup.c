/*
 * strdup.c - planted faults in strdup() and strndup(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strdup.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char phase[16];

/* Own string helpers: nothing here goes through an interposed symbol. */
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

static char sbuf[8192];

char *strdup(const char *s)
{
	REAL(strdup);
	const char *m = mutant_of("strdup.");
	size_t n;
	char *p;

	if (m == NULL)
		return real_strdup(s);
	n = my_len(s);
	if (IS("same_pointer"))
		return (char *)s;
	if (IS("no_nul")) {
		/* The terminator is not copied: a stray byte follows the copy. */
		p = malloc(n + 2);
		if (p != NULL) {
			memcpy(p, s, n);
			p[n] = 'X';
			p[n + 1] = '\0';
		}
		return p;
	}
	if (IS("static_buf") && n < sizeof sbuf) {
		memcpy(sbuf, s, n + 1);
		return sbuf;
	}
	p = real_strdup(s);
	if (p == NULL && IS("enomem_errno"))
		errno = EAGAIN;
	if (p == NULL && IS("enomem_static"))
		return (char *)"";
	return p;
}

/* The number of bytes before a null byte, looking at no more than max. */
static size_t my_nlen(const char *s, size_t max)
{
	size_t n = 0;

	while (n < max && s[n] != '\0')
		n++;
	return n;
}

char *strndup(const char *s, size_t size)
{
	REAL(strndup);
	const char *m = mutant_of("strdup.");
	size_t n;
	char *p;

	if (m == NULL)
		return real_strndup(s, size);
	if (IS("n_static")) {
		n = my_nlen(s, size);
		if (n < sizeof sbuf) {
			memcpy(sbuf, s, n);
			sbuf[n] = '\0';
			return sbuf;
		}
	}
	if (IS("n_drops_last")) {
		n = my_nlen(s, size);
		return real_strndup(s, n > 0 ? n - 1 : 0);
	}
	if (IS("n_ignores_size"))
		return strdup(s);
	if (IS("n_no_terminator")) {
		n = my_nlen(s, size);
		if (n == size) {
			p = malloc(n + 2);
			if (p != NULL) {
				memcpy(p, s, n);
				p[n] = 'X';
				p[n + 1] = '\0';
			}
			return p;
		}
	}
	if (IS("n_uses_strlen")) {
		/* Measures the whole string first, past size. */
		n = my_len(s);
		return real_strndup(s, n < size ? n : size);
	}
	if (IS("n_reads_one_more")) {
		/* Looks at s[size] too (a common off-by-one in the scan). */
		volatile char c = s[size];

		(void)c;
	}
	p = real_strndup(s, size);
	if (p == NULL && IS("n_enomem_errno"))
		errno = EAGAIN;
	if (p == NULL && IS("n_enomem_truncates")) {
		/* Gives back as much as it could allocate: one byte. */
		p = malloc(1);
		if (p != NULL)
			p[0] = '\0';
	}
	return p;
}
