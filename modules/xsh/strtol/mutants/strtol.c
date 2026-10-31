/*
 * strtol.c - planted faults in strtol() and strtoll(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strtol.<fault>" and apply only
 * while PC_PHASE is "test". With no fault planted each function calls the
 * C library's own; with one planted, a small converter of our own runs,
 * correct except for the one thing the fault changes. Both functions (and
 * the names a library may bind them to) get the same fault.
 */
#define _GNU_SOURCE 1
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

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

#define IS(name) (strcmp(m, name) == 0)

static int is_ws(int c, const char *m)
{
	if (IS("ws_none"))
		return 0;
	if (IS("ws_only_space"))
		return c == ' ';
	if (IS("ws_no_vt_ff"))
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	if (IS("ws_no_cr"))
		return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

static int digit_of(int c, const char *m)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'Z')
		return IS("upper_letters_rejected") ? 99 : c - 'A' + 10;
	return 99;
}

static int xdig(int c)
{
	return isxdigit((unsigned char)c) != 0;
}

static long long conv(const char *s, char **e, int base, long long mx, long long mn, const char *m)
{
	const char *p = s, *start, *prev = NULL, *wsend;
	int neg = 0, signed_ = 0, ndig = 0, ovf = 0, d, nws = 0;
	unsigned long long acc = 0, prevacc = 0, lim;
	long long r;
	int lim_n = 0;

	if (e == NULL && IS("null_endptr_zero"))
		return 0;
	if (base < 0 || base == 1 || base > 36) {
		if (base == 37 && IS("base37_accepted"))
			base = 36;
		else if (base == 1 && IS("base1_accepted"))
			base = 10;
		else if (base < 0 && IS("negbase_accepted"))
			base = 10;
		else {
			if (!IS("bad_base_no_errno"))
				errno = IS("bad_base_erange") ? ERANGE : EINVAL;
			return 0;
		}
	}
	while (*p != '\0' && is_ws((unsigned char)*p, m)) {
		if (IS("ws_one_only") && nws++ >= 1)
			break;
		p++;
	}
	wsend = p;
	if (*p == '-' || (*p == '+' && !IS("plus_rejected"))) {
		signed_ = 1;
		if (*p == '-' && !IS("minus_ignored") && !(IS("minus_ignored_base0") && base == 0))
			neg = 1;
		p++;
		if (IS("space_after_sign"))
			while (*p != '\0' && is_ws((unsigned char)*p, m))
				p++;
	}
	start = p;
	{
		int xok = (p[0] == '0' && (p[1] == 'x' || (p[1] == 'X' && !IS("upper_x_rejected"))));
		int xp = xok && !(signed_ && IS("prefix_after_sign_rejected"));

		if (base == 0) {
			if (xp && (xdig(p[2]) || IS("base0_0x_consumed")) && !IS("base0_no_hex")) {
				base = 16;
				p += 2;
			} else if (p[0] == '0')
				base = IS("base0_octal_as_decimal") ? 10 : 8;
			else
				base = 10;
			if (base == 8 && IS("base0_octal_accepts_89"))
				base = 10;
		} else if (base == 16 && xp && (xdig(p[2]) || IS("base16_0x_consumed")) && !IS("hex_prefix_rejected"))
			p += 2;
		else if (base == 10 && p[0] == '0' && IS("base10_leading_zero_octal"))
			base = 8;
	}
	if (neg)
		lim = (unsigned long long)(-(mn + 1)) + 1ULL;
	else
		lim = (unsigned long long)mx;
	if (IS("min_symmetric"))
		lim = (unsigned long long)mx;
	if (IS("max_flagged") && !neg)
		lim_n = -1;
	if (IS("limit_plus_one"))
		lim_n = 1;
	for (;; p++) {
		int c = (unsigned char)*p;
		if (IS("underscore_skipped") && c == '_' && ndig > 0)
			continue;
		if (IS("junk_skipped") && c != '\0' && digit_of(c, m) >= base && ndig > 0)
			continue;
		d = digit_of(c, m);
		if (IS("z_rejected") && d == 35)
			d = 99;
		if (IS("digit_eq_base_ok") ? d > base : d >= base)
			break;
		if (d >= base)
			d = base - 1;	/* digit equal to the base: only for the fault */
		ndig++;
		prevacc = acc;
		prev = p;
		if (!ovf) {
			unsigned long long L = lim + (lim_n > 0 ? 1 : 0);
			unsigned long long lo = (unsigned long long)base;
			if (acc > (L - (unsigned long long)d) / lo
			    || (lim_n < 0 && acc * lo + (unsigned long long)d >= lim)) {
				ovf = 1;
				if (IS("overflow_stops_consuming"))
					break;
			} else
				acc = acc * lo + (unsigned long long)d;
		}
	}
	(void)prev;
	if (ndig == 0) {
		const char *q = s;
		if (IS("nc_end_after_ws"))
			q = wsend;
		if (IS("nc_end_after_sign"))
			q = start;
		if (IS("nc_end_end"))
			q = s + strlen(s);
		if (e != NULL && !IS("nc_end_unset"))
			*e = (char *)q;
		if (IS("nc_errno_erange"))
			errno = ERANGE;
		if (IS("nc_errno_edom"))
			errno = EDOM;
		return IS("nc_nonzero") ? -1 : 0;
	}
	if (e == NULL && IS("null_endptr_crash"))
		*(volatile char *volatile *)e = (char *)p;
	if (e != NULL) {
		const char *q = p;
		if (IS("end_after_junk"))
			q = s + strlen(s);
		else if (IS("end_off_by_one"))
			q = p - 1;
		else if (IS("end_at_start"))
			q = s;
		else if (ovf && IS("overflow_end_at_start"))
			q = s;
		if (!IS("end_unset_on_success"))
			*e = (char *)q;
	}
	if (ovf) {
		if (IS("overflow_no_erange"))
			;
		else if (IS("overflow_einval"))
			errno = EINVAL;
		else
			errno = ERANGE;
		if (IS("overflow_wraps")) {
			/* the whole digit string computed modulo 2^64 */
			unsigned long long w = 0;
			const char *t = start;
			for (; t < p; t++)
				w = w * (unsigned long long)base + (unsigned long long)digit_of((unsigned char)*t, m);
			return neg ? -(long long)w : (long long)w;
		}
		if (IS("overflow_zero"))
			return 0;
		if (IS("overflow_neg_max"))
			return mx;
		if (IS("overflow_pos_min"))
			return mn;
		return neg ? mn : mx;
	}
	if (IS("drop_last_digit") && ndig > 0)
		acc = prevacc;
	if (IS("errno_cleared"))
		errno = 0;
	if (IS("errno_einval_success"))
		errno = EINVAL;
	if (!neg && acc > (unsigned long long)mx)
		return mn;	/* only reachable with limit_plus_one */
	if (neg && acc > (unsigned long long)(-(mn + 1)) + 1ULL)
		return mx;
	if (neg) {
		if (IS("neg_off_by_one"))
			return (long long)(~acc);
		r = (long long)(0ULL - acc);
		return r;
	}
	return (long long)acc;
}

/*
 * Defined under their symbol names with asm labels, since a header may
 * already redirect the plain names to others; the driver finds those
 * (modules/xsh/strtol/probe.c) and passes them as PC_SYM_<name>_<k>.
 */
#define PTYPE(T) T (*)(const char *restrict, char **restrict, int)
#define WRAP(id, sym, symfb, T, MX, MN) \
T id(const char *restrict s, char **restrict e, int b) __asm__(sym); \
T id(const char *restrict s, char **restrict e, int b) \
{ \
	static T (*real)(const char *restrict, char **restrict, int); \
	const char *m = mutant_of("strtol."); \
	if (m != NULL) \
		return (T)conv(s, e, b, MX, MN, m); \
	if (real == NULL && (real = (PTYPE(T))dlsym(RTLD_NEXT, sym)) == NULL) \
		real = (PTYPE(T))dlsym(RTLD_NEXT, symfb); \
	return real(s, e, b); \
}

WRAP(f_strtol, "strtol", "strtol", long, LONG_MAX, LONG_MIN)
#ifdef PC_SYM_strtol_1
WRAP(f_strtol_1, PC_SYM_strtol_1, "strtol", long, LONG_MAX, LONG_MIN)
#endif
#ifdef PC_SYM_strtol_2
WRAP(f_strtol_2, PC_SYM_strtol_2, "strtol", long, LONG_MAX, LONG_MIN)
#endif
#ifdef PC_SYM_strtol_3
WRAP(f_strtol_3, PC_SYM_strtol_3, "strtol", long, LONG_MAX, LONG_MIN)
#endif
WRAP(f_strtoll, "strtoll", "strtoll", long long, LLONG_MAX, LLONG_MIN)
#ifdef PC_SYM_strtoll_1
WRAP(f_strtoll_1, PC_SYM_strtoll_1, "strtoll", long long, LLONG_MAX, LLONG_MIN)
#endif
#ifdef PC_SYM_strtoll_2
WRAP(f_strtoll_2, PC_SYM_strtoll_2, "strtoll", long long, LLONG_MAX, LLONG_MIN)
#endif
#ifdef PC_SYM_strtoll_3
WRAP(f_strtoll_3, PC_SYM_strtoll_3, "strtoll", long long, LLONG_MAX, LLONG_MIN)
#endif
