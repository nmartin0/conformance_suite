/*
 * fprintf.c - conforming variants of the fprintf() family, for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. A variant applies to every function of the family
 * (asprintf, dprintf, fprintf, printf, snprintf, sprintf and their v
 * forms), in every phase and process, as a system's own library would.
 * The formatting variants run through ../mutants/fmt.inc, which leaves
 * every other conversion to the C library.
 */
#define _GNU_SOURCE 1
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include "pcvariant.h"
#include "../mutants/fmt.inc"

static const char *v;	/* the active variant, for the hooks */

/*
 * "inf_long": an infinity is written "infinity" ("INFINITY" for F, E, G
 * and A). fprintf() f, F: which of the two styles is implementation-defined;
 * e, g and a use the style of f.
 * "nan_seq": a NaN is written "nan(7)" ("NAN(7)"), with a '-' when its sign
 * is negative. fprintf() f, F: the style with an n-char-sequence and its
 * meaning are implementation-defined.
 * "p_style": a pointer is written "P:" and 16 upper-case hexadecimal digits.
 * fprintf() p: the characters are printable and otherwise
 * implementation-defined.
 * "a_lead8": a normalized double in a or A style has a leading hexadecimal
 * digit from 8 to f (0x8p-3 for 1), correctly rounded at the given
 * precision. fprintf() a, A: the digit before the radix character is
 * non-zero for a normalized number and otherwise unspecified.
 * "styles_differ" (a READING of XSH.fprintf.058): f and F write infinities
 * and NaNs as the library does, but e, g and a write an infinity
 * "infinity" ("INFINITY") and a NaN "nan(1)" ("NAN(1)"). fprintf() f, F:
 * the styles and the meaning of the n-char-sequence are
 * implementation-defined; e, g and a are only required to use "the style of
 * an f or F conversion specifier", which can be read as any of the f
 * styles.
 * "dprintf_bytewise": dprintf() and vdprintf() write their output one byte
 * per write(). fprintf() dprintf(): equivalent to fprintf() writing to the
 * descriptor; how many write() calls that takes is not specified.
 */
static int engine(void)
{
	return IS("inf_long") || IS("nan_seq") || IS("p_style") || IS("a_lead8") || IS("styles_differ");
}

/* Pads body (sign and prefix first, nb bytes of them) to the field width. */
static void padded(struct fe_out *o, const struct fe_spec *sp, const char *body, size_t n,
    size_t nb, int zeros)
{
	size_t w = sp->width > 0 ? (size_t)sp->width : 0, i;

	if (n >= w || sp->minus) {
		fe_put(o, body, n);
		for (i = n; i < w; i++)
			fe_put(o, " ", 1);
		return;
	}
	if (zeros) {
		fe_put(o, body, nb);
		for (i = n; i < w; i++)
			fe_put(o, "0", 1);
		fe_put(o, body + nb, n - nb);
		return;
	}
	for (i = n; i < w; i++)
		fe_put(o, " ", 1);
	fe_put(o, body, n);
}

static int upper_conv(char c)
{
	return c == 'F' || c == 'E' || c == 'G' || c == 'A';
}

static int lead8(struct fe_out *o, const struct fe_spec *sp, double d)
{
	static const char lo[] = "0123456789abcdef", up[] = "0123456789ABCDEF";
	const char *hx = sp->conv == 'A' ? up : lo;
	uint64_t bits, n;
	int e, neg, x, nd = 13, P = sp->prec, i;
	char small[160], *body = small, *q;
	size_t nb;

	if (sizeof d != 8 || DBL_MANT_DIG != 53 || FLT_RADIX != 2 || DBL_MAX_EXP != 1024)
		return 0;
	memcpy(&bits, &d, 8);
	neg = (int)(bits >> 63);
	e = (int)((bits >> 52) & 0x7ff);
	if (e == 0 || e == 0x7ff)
		return 0;	/* zero, subnormal, infinity, NaN: as the library does */
	n = ((UINT64_C(1) << 52) | (bits & ((UINT64_C(1) << 52) - 1))) << 3;
	x = e - 1023 - 3;
	if (P >= 0 && P < 13) {
		int sh = 4 * (13 - P);
		uint64_t qv = n >> sh, rem = n & ((UINT64_C(1) << sh) - 1), half = UINT64_C(1) << (sh - 1);

		if (rem > half || (rem == half && (qv & 1)))
			qv++;
		if ((qv >> (4 * P)) >= 16) {
			qv >>= 4;
			x += 4;
		}
		n = qv;
		nd = P;
	} else if (P < 0) {
		while (nd > 0 && (n & 0xf) == 0) {
			n >>= 4;
			nd--;
		}
	}
	if (P > 100 && (body = malloc((size_t)P + 64)) == NULL) {
		fe_fail(o, ENOMEM);
		return 1;
	}
	q = body;
	if (neg)
		*q++ = '-';
	else if (sp->plus)
		*q++ = '+';
	else if (sp->space)
		*q++ = ' ';
	*q++ = '0';
	*q++ = sp->conv == 'A' ? 'X' : 'x';
	nb = (size_t)(q - body);
	*q++ = hx[(n >> (4 * nd)) & 0xf];
	if (nd > 0 || P > 0 || sp->alt)
		*q++ = '.';
	for (i = nd - 1; i >= 0; i--)
		*q++ = hx[(n >> (4 * i)) & 0xf];
	for (i = 13; i < P; i++)
		*q++ = '0';
	*q++ = sp->conv == 'A' ? 'P' : 'p';
	*q++ = x < 0 ? '-' : '+';
	q += fe_real()(q, 12, "%d", x < 0 ? -x : x);
	padded(o, sp, body, (size_t)(q - body), nb, sp->zero);
	if (body != small)
		free(body);
	return 1;
}

/* ---------- hooks ---------- */

static int fe_hook_text(struct fe_out *o, const char *s, size_t n, int last)
{
	(void)o; (void)s; (void)n; (void)last;
	return 0;
}

static int fe_hook_percent(struct fe_out *o, struct fe_args *a)
{
	(void)o; (void)a;
	return 0;
}

static void fe_hook_stars(struct fe_spec *sp, int *w, int *p)
{
	(void)sp; (void)w; (void)p;
}

static void fe_hook_spec(struct fe_spec *sp, union fe_val *val)
{
	(void)sp; (void)val;
}

static void fe_hook_type(struct fe_spec *sp)
{
	(void)sp;
}

static int fe_hook_fail(struct fe_out *o, const struct fe_spec *sp, union fe_val val)
{
	(void)o; (void)sp; (void)val;
	return 0;
}

static void fe_hook_n(struct fe_out *o, struct fe_spec *sp, size_t *count)
{
	(void)o; (void)sp; (void)count;
}

static int fe_hook_posmode(void)
{
	return 0;
}

static int fe_hook_piece(struct fe_out *o, const struct fe_spec *sp, union fe_val val,
    const char *piece, size_t n)
{
	char body[64];
	size_t k = 0;

	(void)piece; (void)n;
	if (fe_isfloat(sp->conv)) {
		long double d = sp->type == FE_LDBL ? val.ld : (long double)val.d;
		int up = upper_conv(sp->conv), neg = signbit(d) != 0;
		int other = IS("styles_differ") && sp->conv != 'f' && sp->conv != 'F';

		if (d == d && d - d != d - d && (IS("inf_long") || other)) {
			if (neg)
				body[k++] = '-';
			else if (sp->plus)
				body[k++] = '+';
			else if (sp->space)
				body[k++] = ' ';
			memcpy(body + k, up ? "INFINITY" : "infinity", 8);
			padded(o, sp, body, k + 8, 0, 0);
			return 1;
		}
		if (d != d && (IS("nan_seq") || other)) {
			if (neg)
				body[k++] = '-';
			else if (sp->plus)
				body[k++] = '+';
			else if (sp->space)
				body[k++] = ' ';
			memcpy(body + k, other ? (up ? "NAN(1)" : "nan(1)") : (up ? "NAN(7)" : "nan(7)"), 6);
			padded(o, sp, body, k + 6, 0, 0);
			return 1;
		}
		if ((sp->conv == 'a' || sp->conv == 'A') && sp->type == FE_DBL && IS("a_lead8"))
			return lead8(o, sp, val.d);
		return 0;
	}
	if (sp->conv == 'p' && IS("p_style")) {
		uintmax_t u = (uintmax_t)(uintptr_t)val.p;
		int i;

		body[k++] = 'P';
		body[k++] = ':';
		for (i = 15; i >= 0; i--)
			body[k++] = "0123456789ABCDEF"[(u >> (4 * i)) & 0xf];
		padded(o, sp, body, k, 0, 0);
		return 1;
	}
	return 0;
}

/* ---------- the functions ---------- */

static int run(const char *fmt, va_list ap, char **res, size_t *len)
{
	va_list cp;
	int r;

	va_copy(cp, ap);
	r = fe_run(fmt, &cp, res, len);
	va_end(cp);
	return r;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	REAL(vsnprintf);
	char *b;
	size_t len, k;

	v = variant_of("fprintf.");
	if (!engine())
		return real_vsnprintf(s, n, fmt, ap);
	if (run(fmt, ap, &b, &len) != 0)
		return -1;
	if (n > 0) {
		k = len < n - 1 ? len : n - 1;
		memcpy(s, b, k);
		s[k] = '\0';
	}
	free(b);
	return (int)len;
}

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return r;
}

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	REAL(vsprintf);
	char *b;
	size_t len;

	v = variant_of("fprintf.");
	if (!engine())
		return real_vsprintf(s, fmt, ap);
	if (run(fmt, ap, &b, &len) != 0)
		return -1;
	memcpy(s, b, len + 1);
	free(b);
	return (int)len;
}

int sprintf(char *restrict s, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vsprintf(s, fmt, ap);
	va_end(ap);
	return r;
}

static int to_stream(FILE *f, const char *fmt, va_list ap)
{
	char *b;
	size_t len, w;

	if (run(fmt, ap, &b, &len) != 0)
		return -1;
	w = fwrite(b, 1, len, f);
	free(b);
	return w < len ? -1 : (int)len;
}

int vfprintf(FILE *restrict f, const char *restrict fmt, va_list ap)
{
	REAL(vfprintf);

	v = variant_of("fprintf.");
	if (!engine())
		return real_vfprintf(f, fmt, ap);
	return to_stream(f, fmt, ap);
}

int fprintf(FILE *restrict f, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfprintf(f, fmt, ap);
	va_end(ap);
	return r;
}

int vprintf(const char *restrict fmt, va_list ap)
{
	REAL(vprintf);

	v = variant_of("fprintf.");
	if (!engine())
		return real_vprintf(fmt, ap);
	return to_stream(stdout, fmt, ap);
}

int printf(const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vprintf(fmt, ap);
	va_end(ap);
	return r;
}

int vdprintf(int fd, const char *restrict fmt, va_list ap)
{
	REAL(vdprintf);
	char *b;
	size_t len, done = 0;
	int bytewise;

	v = variant_of("fprintf.");
	bytewise = IS("dprintf_bytewise");
	if (!engine() && !bytewise)
		return real_vdprintf(fd, fmt, ap);
	if (bytewise) {
		va_list cp;
		REAL(vsnprintf);
		int k;

		va_copy(cp, ap);
		k = real_vsnprintf(NULL, 0, fmt, cp);
		va_end(cp);
		if (k < 0)
			return -1;
		b = malloc((size_t)k + 1);
		if (b == NULL) {
			errno = ENOMEM;
			return -1;
		}
		va_copy(cp, ap);
		(void)real_vsnprintf(b, (size_t)k + 1, fmt, cp);
		va_end(cp);
		len = (size_t)k;
	} else if (run(fmt, ap, &b, &len) != 0) {
		return -1;
	}
	while (done < len) {
		ssize_t w = write(fd, b + done, bytewise ? 1 : len - done);

		if (w < 0) {
			if (errno == EINTR)
				continue;
			free(b);
			return -1;
		}
		done += (size_t)w;
	}
	free(b);
	return (int)len;
}

int dprintf(int fd, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vdprintf(fd, fmt, ap);
	va_end(ap);
	return r;
}

int vasprintf(char **restrict ptr, const char *restrict fmt, va_list ap)
{
	REAL(vasprintf);
	char *b;
	size_t len;

	v = variant_of("fprintf.");
	if (!engine())
		return real_vasprintf(ptr, fmt, ap);
	if (run(fmt, ap, &b, &len) != 0)
		return -1;
	*ptr = b;
	return (int)len;
}

int asprintf(char **restrict ptr, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vasprintf(ptr, fmt, ap);
	va_end(ap);
	return r;
}
