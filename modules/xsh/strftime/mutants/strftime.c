/*
 * strftime.c - planted faults in strftime() and strftime_l(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strftime.<fault>" and apply only
 * while PC_PHASE is "test". Most faults in a conversion are planted by
 * handing the real function a format in which that conversion is replaced
 * by what the faulty implementation would produce.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "strftime.", 9) != 0)
		return NULL;
	return m + 9;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

typedef size_t (*sf_fn)(char *, size_t, const char *, const struct tm *);
typedef size_t (*sfl_fn)(char *, size_t, const char *, const struct tm *, locale_t);

static sf_fn real_sf(void)
{
	static sf_fn fn;

	if (fn == NULL)
		fn = (sf_fn)dlsym(RTLD_NEXT, "strftime");
	return fn;
}

static sfl_fn real_sfl(void)
{
	static sfl_fn fn;

	if (fn == NULL)
		fn = (sfl_fn)dlsym(RTLD_NEXT, "strftime_l");
	return fn;
}

/* The XBD 4.19 value of the members, as if they were UTC. */
static long long as_utc(const struct tm *tm)
{
	long long y = tm->tm_year;

	return tm->tm_sec + tm->tm_min * 60LL + tm->tm_hour * 3600LL + tm->tm_yday * 86400LL +
	    (y - 70) * 31536000LL + ((y - 69) / 4) * 86400LL - ((y - 1) / 100) * 86400LL +
	    ((y + 299) / 400) * 86400LL;
}

/*
 * The replacement for one conversion specification spec (from '%' to the
 * specifier, flags and width included) under fault m, or NULL to keep it.
 * Literal text in the replacement has its '%' doubled by the caller.
 */
static const char *subst(const char *m, const char *spec, size_t len, const struct tm *tm, char *lit,
    size_t litsize, int *literal)
{
	char c = spec[len - 1];
	int plain = len == 2;

	*literal = 0;
	if (IS("abday_full") && plain && c == 'a')
		return "%A";
	if (IS("abmon_full") && plain && (c == 'b' || c == 'h'))
		return "%B";
	if (IS("pm_noon") && plain && c == 'p' && tm->tm_hour == 12)
		return "AM";
	if (IS("e_zero") && plain && c == 'e')
		return "%d";
	if (IS("I_zero") && plain && c == 'I' && tm->tm_hour == 0)
		return "00";
	if (IS("j_zero") && plain && c == 'j') {
		snprintf(lit, litsize, "%03d", tm->tm_yday);	/* counted from 0 */
		*literal = 1;
		return lit;
	}
	if (IS("C_single") && plain && c == 'C') {
		snprintf(lit, litsize, "%d", (tm->tm_year + 1900) / 100);	/* no leading zero */
		*literal = 1;
		return lit;
	}
	if (IS("C_trunc") && plain && c == 'C') {
		snprintf(lit, litsize, "%02d", ((tm->tm_year + 1900) / 100) % 100);	/* two digits only */
		*literal = 1;
		return lit;
	}
	if (IS("Y_four") && plain && (c == 'Y' || c == 'G')) {
		snprintf(lit, litsize, "%04d", (tm->tm_year + 1900) % 10000);	/* four digits only */
		*literal = 1;
		return lit;
	}
	if (IS("c_order") && plain && c == 'c')
		return "%a %e %b %H:%M:%S %Y";
	if (IS("D_dmy") && plain && c == 'D')
		return "%d/%m/%y";
	if (IS("r_24") && plain && c == 'r')
		return "%H:%M:%S %p";
	if (IS("x_long") && plain && c == 'x')
		return "%m/%d/%Y";
	if (IS("U_as_W") && plain && c == 'U')
		return "%W";
	if (IS("u_zero") && plain && c == 'u')
		return "%w";
	if (IS("W_off") && plain && c == 'W')
		return "%U";
	if (IS("G_calendar") && plain && c == 'G')
		return "%Y";
	if (IS("G_calendar") && plain && c == 'g')
		return "%y";
	if (IS("V_as_W") && plain && c == 'V')
		return "%W";
	if (IS("E_empty") && len == 3 && spec[1] == 'E' && c == 'c')
		return "";
	if (IS("O_space") && len == 3 && spec[1] == 'O' && c == 'd')
		return "%e";
	if (IS("OB_abbrev") && len == 3 && spec[1] == 'O' && c == 'B')
		return "%b";
	if (IS("plus_as_zero") && spec[1] == '+') {
		snprintf(lit, litsize, "%%0%.*s", (int)(len - 2), spec + 2);
		return lit;
	}
	if (IS("plus_always") && spec[1] == '+' && c == 'Y') {
		int w = atoi(spec + 2);

		/* The sign whenever the flag is given, whatever the length. */
		snprintf(lit, litsize, "+%0*lld", w > 1 ? w - 1 : 1, tm->tm_year + 1900LL);
		*literal = 1;
		return lit;
	}
	if (IS("width_ignored") && (spec[1] == '+' || spec[1] == '0')) {
		snprintf(lit, litsize, "%%%c", c);
		return lit;
	}
	if (IS("s_utc") && plain && c == 's') {
		snprintf(lit, litsize, "%lld", as_utc(tm));	/* TZ and tm_isdst ignored */
		*literal = 1;
		return lit;
	}
	if (IS("s_isdst_ignored") && plain && c == 's') {
		struct tm t = *tm;

		t.tm_isdst = -1;
		snprintf(lit, litsize, "%lld", (long long)mktime(&t));
		*literal = 1;
		return lit;
	}
	if (IS("z_std") && plain && c == 'z' && tm->tm_isdst >= 0) {
		long e = -timezone;	/* always the standard time offset */

		snprintf(lit, litsize, "%c%02ld%02ld", e < 0 ? '-' : '+', labs(e) / 3600, labs(e) % 3600 / 60);
		*literal = 1;
		return lit;
	}
	if (IS("z_sign") && plain && c == 'z' && tm->tm_isdst >= 0) {
		long e = tm->tm_gmtoff;	/* east of UTC printed as west */

		snprintf(lit, litsize, "%c%02ld%02ld", e > 0 ? '-' : '+', labs(e) / 3600, labs(e) % 3600 / 60);
		*literal = 1;
		return lit;
	}
	if (IS("z_neg_utc") && plain && c == 'z' && tm->tm_isdst < 0)
		return "+0000";
	return NULL;
}

/* The format with each conversion replaced as fault m says. */
static const char *rewrite(const char *m, const char *fmt, const struct tm *tm, char *out, size_t size)
{
	size_t o = 0, len;
	const char *p = fmt, *r;
	char lit[64];
	int literal;

	while (*p != '\0' && o + 80 < size) {
		if (*p != '%') {
			out[o++] = *p++;
			continue;
		}
		len = 1;
		if (p[1] == '%') {
			out[o++] = '%';
			out[o++] = '%';
			p += 2;
			continue;
		}
		while (p[len] == '+' || p[len] == '0')
			len++;
		while (p[len] >= '0' && p[len] <= '9')
			len++;
		if (p[len] == 'E' || p[len] == 'O')
			len++;
		if (p[len] == '\0')
			break;
		len++;
		r = subst(m, p, len, tm, lit, sizeof lit, &literal);
		if (r == NULL) {
			memcpy(out + o, p, len);
			o += len;
		} else {
			for (; *r != '\0' && o + 2 < size; r++) {
				if (literal && *r == '%')
					out[o++] = '%';
				out[o++] = *r;
			}
		}
		p += len;
	}
	out[o] = '\0';
	return out;
}

static char saved0[64], saved1[64];

/* The common path of both functions; loc is NULL for strftime(). */
static size_t fault(const char *m, char *s, size_t max, const char *fmt, const struct tm *tm, locale_t loc)
{
	char f[2048], tmp[4096];
	size_t n;
	int e = errno;

	fmt = rewrite(m, fmt, tm, f, sizeof f);
	if (IS("keeps_tzname")) {
		snprintf(saved0, sizeof saved0, "%s", tzname[0] ? tzname[0] : "");
		snprintf(saved1, sizeof saved1, "%s", tzname[1] ? tzname[1] : "");
	}
	n = loc ? real_sfl()(s, max, fmt, tm, loc) : real_sf()(s, max, fmt, tm);
	if (IS("keeps_tzname")) {
		tzname[0] = saved0;
		tzname[1] = saved1;
	}
	if (n > 0) {
		if (IS("errno_zero"))
			errno = 0;
		if (IS("errno_erange"))
			errno = ERANGE;
		return n;
	}
	if (IS("erange_einval"))
		errno = EINVAL;
	if (IS("overrun") && max > 0)
		s[max] = 'X';	/* one byte past the array */
	if (IS("no_nul_room") && max > 0 && max < sizeof tmp) {
		/* Room is kept for the bytes but not for the NUL. */
		n = loc ? real_sfl()(tmp, max + 1, fmt, tm, loc) : real_sf()(tmp, max + 1, fmt, tm);
		if (n == max) {
			memcpy(s, tmp, max);
			errno = e;
			return n;
		}
		return 0;
	}
	return n;
}

size_t strftime(char *restrict s, size_t max, const char *restrict fmt, const struct tm *restrict tm)
{
	const char *m = mutant();

	if (m == NULL)
		return real_sf()(s, max, fmt, tm);
	return fault(m, s, max, fmt, tm, (locale_t)0);
}

size_t strftime_l(char *restrict s, size_t max, const char *restrict fmt, const struct tm *restrict tm,
    locale_t loc)
{
	const char *m = mutant();
	char f[2048];
	size_t n;

	if (m == NULL)
		return real_sfl()(s, max, fmt, tm, loc);
	if (IS("l_abday_full"))
		return real_sfl()(s, max, rewrite("abday_full", fmt, tm, f, sizeof f), tm, loc);
	n = fault(m, s, max, fmt, tm, loc);
	if (IS("l_counts_nul") && n > 0)
		n++;	/* the NUL counted */
	return n;
}
