/*
 * strerror.c - conforming variants of strerror(), strerror_l() and
 * strerror_r(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. The POSIX strerror_r() is defined under both of its
 * possible symbols (see mutants/strerror.c).
 *
 * The modelled system's error numbers are the positive ones below
 * INT_MAX; -1, INT_MIN and INT_MAX are not error numbers there.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <string.h>
#include "pcvariant.h"

static char *(*real_strerror)(int);
static char *(*real_strerror_l)(int, locale_t);
static int (*real_xpg)(int, char *, size_t);

static void init(void)
{
	if (real_strerror == NULL) {
		real_strerror = (char *(*)(int))dlsym(RTLD_NEXT, "strerror");
		real_strerror_l = (char *(*)(int, locale_t))dlsym(RTLD_NEXT, "strerror_l");
		real_xpg = (int (*)(int, char *, size_t))dlsym(RTLD_NEXT, "__xpg_strerror_r");
		if (real_xpg == NULL)
			real_xpg = (int (*)(int, char *, size_t))dlsym(RTLD_NEXT, "strerror_r");
	}
}

static int invalid(int e)
{
	return e < 0 || e == INT_MAX;
}

/*
 * "einval_empty": for a number that is not an error number, strerror()
 * and strerror_l() set errno to EINVAL and return an empty string, and
 * strerror_r() returns EINVAL without storing anything. ERRORS makes
 * EINVAL a may-fail error for these functions, RETURN VALUE allows errno
 * to be set while a string is still returned, and RATIONALE names the
 * empty string as one permitted result.
 * "einval_text": the same EINVAL, but with an unknown-error text returned
 * (stored, for strerror_r(), when it fits): RATIONALE's third outcome.
 * "zero_empty": the message for 0 is the empty string, which DESCRIPTION
 * allows for errnum 0.
 * "r_errno": a successful strerror_r() changes errno. The rule that errno
 * is kept on success names strerror() and strerror_l() only, and errno is
 * otherwise unspecified after a successful call (XSH errno).
 * "r_untouched": when the buffer is too small, strerror_r() returns
 * ERANGE and stores nothing. ERRORS requires only the ERANGE return;
 * terminating the buffer is merely encouraged (RATIONALE).
 * "unknown_empty": for a number that is not an error number, strerror()
 * and strerror_l() return an empty string and leave errno alone: the
 * EINVAL condition is detected (an error, so no unknown-error text is
 * owed: DESCRIPTION requires that text only on successful completion)
 * but errno is not set, which RETURN VALUE's "may be set" allows; a
 * string is still returned, and RATIONALE names the empty string.
 * "global_buf": strerror() copies its message into one buffer for the
 * whole process, and strerror_l() into one buffer per thread. DESCRIPTION
 * lets strerror() overwrite its string on any later strerror() call, and
 * strerror_l() on a later call in the same thread.
 */
static char global[512];
static _Thread_local char perthread[512];

static char *keep(char *buf, size_t size, const char *s)
{
	size_t n = strlen(s);

	if (n >= size)
		n = size - 1;
	memcpy(buf, s, n);
	buf[n] = '\0';
	return buf;
}

char *strerror(int e)
{
	const char *v = variant_of("strerror.");
	int saved = errno;
	char *s;

	init();
	s = real_strerror(e);
	errno = saved;
	if (v == NULL)
		return s;
	if (IS("einval_empty") && invalid(e)) {
		errno = EINVAL;
		return (char *)"";
	}
	if (IS("einval_text") && invalid(e))
		errno = EINVAL;
	if (IS("zero_empty") && e == 0)
		return (char *)"";
	if (IS("unknown_empty") && invalid(e))
		return (char *)"";
	if (IS("global_buf"))
		return keep(global, sizeof global, s);
	return s;
}

char *strerror_l(int e, locale_t loc)
{
	const char *v = variant_of("strerror.");
	int saved = errno;
	char *s;

	init();
	s = real_strerror_l(e, loc);
	errno = saved;
	if (v == NULL)
		return s;
	if (IS("einval_empty") && invalid(e)) {
		errno = EINVAL;
		return (char *)"";
	}
	if (IS("einval_text") && invalid(e))
		errno = EINVAL;
	if (IS("zero_empty") && e == 0)
		return (char *)"";
	if (IS("unknown_empty") && invalid(e))
		return (char *)"";
	if (IS("global_buf"))
		return keep(perthread, sizeof perthread, s);
	return s;
}

static int var_r(int e, char *buf, size_t len)
{
	const char *v = variant_of("strerror.");
	const char *msg;
	size_t n;
	int r;

	init();
	if (v == NULL)
		return real_xpg(e, buf, len);
	if (IS("einval_empty") && invalid(e))
		return EINVAL;
	if (IS("einval_text") && invalid(e)) {
		msg = real_strerror(e);
		if (strlen(msg) < len)
			memcpy(buf, msg, strlen(msg) + 1);
		return EINVAL;
	}
	if (IS("zero_empty") && e == 0) {
		if (len == 0)
			return ERANGE;
		buf[0] = '\0';
		return 0;
	}
	if (IS("r_untouched")) {
		msg = real_strerror(e);
		n = strlen(msg);
		if (n >= len)
			return ERANGE;
	}
	r = real_xpg(e, buf, len);
	if (IS("r_errno") && r == 0)
		errno = ENOTTY;
	return r;
}

int pcv_strerror_r(int e, char *buf, size_t len) __asm__("strerror_r");
int pcv_xpg_strerror_r(int e, char *buf, size_t len) __asm__("__xpg_strerror_r");

int pcv_strerror_r(int e, char *buf, size_t len)
{
	return var_r(e, buf, len);
}

int pcv_xpg_strerror_r(int e, char *buf, size_t len)
{
	return var_r(e, buf, len);
}
