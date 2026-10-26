/*
 * strerror.c - planted faults in strerror(), strerror_l(), strerror_r()
 * and perror(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strerror.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 *
 * The test program calls strerror_r() under either of two symbols: the
 * plain name, or __xpg_strerror_r where the C library's header renames
 * the POSIX form (glibc). Both are defined here with the POSIX meaning,
 * under assembler names so that the header's own declaration of the
 * plain name does not clash.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
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

static int my_prefix(const char *s, const char *p, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		if (s[i] != p[i])
			return 0;
	return 1;
}

static void my_copy(char *d, const char *s, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		d[i] = s[i];
}

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (my_eq(name, "PC_PHASE") && my_len(value) < sizeof phase)
		my_copy(phase, value, my_len(value) + 1);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = my_len(prefix);

	if (!my_eq(phase, "test") || m == NULL || !my_prefix(m, prefix, n))
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && my_eq(m, name))

static char *(*real_strerror)(int);
static char *(*real_strerror_l)(int, locale_t);
static int (*real_xpg)(int, char *, size_t);
static void (*real_perror)(const char *);

static void init(void)
{
	if (real_strerror == NULL) {
		real_strerror = (char *(*)(int))dlsym(RTLD_NEXT, "strerror");
		real_strerror_l = (char *(*)(int, locale_t))dlsym(RTLD_NEXT, "strerror_l");
		real_xpg = (int (*)(int, char *, size_t))dlsym(RTLD_NEXT, "__xpg_strerror_r");
		if (real_xpg == NULL)
			real_xpg = (int (*)(int, char *, size_t))dlsym(RTLD_NEXT, "strerror_r");
		real_perror = (void (*)(const char *))dlsym(RTLD_NEXT, "perror");
	}
}

/* The numbers the tests use as invalid. */
static int invalid(int e)
{
	return e < 0 || e == INT_MAX;
}

/* One static buffer shared by the faulty functions. */
static char shared[512];

static char *into_shared(const char *msg)
{
	size_t n = my_len(msg);

	if (n >= sizeof shared)
		n = sizeof shared - 1;
	my_copy(shared, msg, n);
	shared[n] = '\0';
	return shared;
}

char *strerror(int e)
{
	const char *m = mutant_of("strerror.");
	int saved = errno;
	char *s;

	init();
	s = real_strerror(e);
	if (m == NULL)
		return s;
	errno = saved;
	if (IS("null_invalid") && invalid(e))
		return NULL;
	if (IS("null_zero") && e == 0)
		return NULL;
	if (IS("errno_clobbered"))
		errno = EINVAL;
	if (IS("empty_valid") && e > 0 && !invalid(e))
		return (char *)"";
	if (IS("zero_einval") && e == 0)
		errno = EINVAL;
	if (IS("invalid_erange") && invalid(e))
		errno = ERANGE;
	if (IS("perror_overwrites") || IS("r_shares_buffer") || IS("l_shares_buffer"))
		return into_shared(s);
	return s;
}

char *strerror_l(int e, locale_t loc)
{
	const char *m = mutant_of("strerror.");
	int saved = errno;
	char *s;

	init();
	s = real_strerror_l(e, loc);
	if (m == NULL)
		return s;
	errno = saved;
	if (IS("l_shares_buffer"))
		return into_shared(s);	/* one buffer for every thread */
	if (IS("l_null") && !invalid(e) && e != 0)
		return NULL;
	if (IS("l_errno_clobbered"))
		errno = ENOENT;
	if (IS("l_empty") && !invalid(e) && e != 0)
		return (char *)"";
	if (IS("l_invalid_null") && invalid(e)) {
		errno = EINVAL;
		return NULL;
	}
	if (IS("l_invalid_erange") && invalid(e))
		errno = ERANGE;	/* an error number the text does not allow here */
	return s;
}

static int faulty_r(int e, char *buf, size_t len)
{
	const char *m = mutant_of("strerror.");
	const char *msg;
	size_t n;
	int r;

	init();
	if (m == NULL)
		return real_xpg(e, buf, len);
	msg = real_strerror(e);
	n = my_len(msg);
	if (IS("r_shares_buffer"))
		(void)into_shared(msg);
	if (IS("r_minus1") && invalid(e)) {
		errno = EINVAL;
		return -1;
	}
	if (IS("r_empty") && !invalid(e) && e != 0 && len > 0) {
		buf[0] = '\0';
		return 0;
	}
	if (IS("r_unterminated") && n < len) {
		size_t i;

		my_copy(buf, msg, n);
		for (i = n; i < len; i++)
			buf[i] = 'X';
		return 0;
	}
	if (IS("r_zero_einval") && e == 0)
		return EINVAL;
	if (IS("r_invalid_empty") && invalid(e) && len > 0) {
		buf[0] = '\0';
		return 0;
	}
	if (n >= len) {
		if (IS("r_no_erange") && len > 0) {
			my_copy(buf, msg, len - 1);
			buf[len - 1] = '\0';
			return 0;
		}
		if (IS("r_overflow")) {
			my_copy(buf, msg, n + 1);
			return ERANGE;
		}
		if (IS("r_off_by_one") && n == len) {
			my_copy(buf, msg, n);
			return 0;
		}
		if (IS("r_minus1")) {
			errno = ERANGE;
			return -1;
		}
	}
	r = real_xpg(e, buf, len);
	return r;
}

int pcm_strerror_r(int e, char *buf, size_t len) __asm__("strerror_r");
int pcm_xpg_strerror_r(int e, char *buf, size_t len) __asm__("__xpg_strerror_r");

int pcm_strerror_r(int e, char *buf, size_t len)
{
	return faulty_r(e, buf, len);
}

int pcm_xpg_strerror_r(int e, char *buf, size_t len)
{
	return faulty_r(e, buf, len);
}

void perror(const char *s)
{
	const char *m = mutant_of("strerror.");

	init();
	if (IS("perror_overwrites")) {
		/* The historical way: through strerror() and its buffer. */
		const char *msg = strerror(errno);

		if (s != NULL && *s != '\0')
			fprintf(stderr, "%s: %s\n", s, msg);
		else
			fprintf(stderr, "%s\n", msg);
		return;
	}
	real_perror(s);
}
