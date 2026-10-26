/*
 * fscanf.c - conforming variants of fscanf(), scanf() and sscanf(), for
 * the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. glibc compiles calls in strict modes to __isoc99_*
 * names, so those are interposed as well (asm labels: under _GNU_SOURCE
 * glibc's <stdio.h> renames the plain names itself).
 */
#define _GNU_SOURCE 1
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pcvariant.h"

typedef int (*vs_fn)(const char *, const char *, va_list);
typedef int (*vf_fn)(FILE *, const char *, va_list);
typedef int (*vsn_fn)(char *, size_t, const char *, va_list);

static void *real_of(const char *a, const char *b)
{
	void *f = dlsym(RTLD_NEXT, a);

	return f != NULL ? f : dlsym(RTLD_NEXT, b);
}

/* One unit in the last place away from v, upwards in magnitude. */
static void bump_float(float *p)
{
	uint32_t u;

	memcpy(&u, p, sizeof u);
	if ((u & 0x7f800000u) != 0x7f800000u && (u & 0x7fffffffu) != 0)
		u++;
	memcpy(p, &u, sizeof u);
}

static void bump_double(double *p)
{
	uint64_t u;

	memcpy(&u, p, sizeof u);
	if ((u & 0x7ff0000000000000u) != 0x7ff0000000000000u && (u & 0x7fffffffffffffffu) != 0)
		u++;
	memcpy(p, &u, sizeof u);
}

static void bump_ldouble(long double *p)
{
	if (sizeof(long double) == sizeof(double)) {
		bump_double((double *)p);
	} else {
		/* x87 extended: the 64-bit significand comes first */
		uint64_t u;

		memcpy(&u, p, sizeof u);
		if (*p == *p && *p != 0 && *p - *p == 0 && (u & 0xffffffffu) != 0xffffffffu)
			u++;
		memcpy(p, &u, sizeof u);
	}
}

/*
 * "float_ulp": every floating result is one unit in the last place larger
 * in magnitude than the reference systems give. ISO C, to which this page
 * defers, makes the accuracy of the conversions between strings and
 * floating types done by <stdio.h> functions implementation-defined, and
 * this page states no accuracy of its own.
 *
 * The results are found by walking the format: the first r assigning
 * conversions succeeded (r is the return value); "%n$" formats are left
 * alone.
 */
static void bump_results(const char *fmt, int r, va_list ap)
{
	const char *p;
	int done = 0;

	if (r <= 0 || strchr(fmt, '$') != NULL)
		return;
	for (p = fmt; *p != '\0' && done < r; p++) {
		int suppress = 0;
		char len[3] = "";
		void *t;

		if (*p != '%')
			continue;
		p++;
		if (*p == '%')
			continue;
		if (*p == '*') {
			suppress = 1;
			p++;
		}
		while (isdigit((unsigned char)*p))
			p++;
		if (*p == 'm')
			p++;
		if ((p[0] == 'h' && p[1] == 'h') || (p[0] == 'l' && p[1] == 'l')) {
			len[0] = p[0];
			len[1] = p[1];
			p += 2;
		} else if (*p != '\0' && strchr("hljztL", *p) != NULL) {
			len[0] = *p++;
		}
		if (*p == '\0')
			break;
		if (*p == '[') {
			p++;
			if (*p == '^')
				p++;
			if (*p == ']')
				p++;
			while (*p != '\0' && *p != ']')
				p++;
			if (*p == '\0')
				break;
		}
		if (suppress)
			continue;
		t = va_arg(ap, void *);
		if (*p == 'n')
			continue;
		done++;
		if (strchr("aAeEfFgG", *p) == NULL)
			continue;
		if (strcmp(len, "l") == 0)
			bump_double(t);
		else if (strcmp(len, "L") == 0)
			bump_ldouble(t);
		else
			bump_float(t);
	}
}

static int after(int r, const char *fmt, va_list ap)
{
	const char *v = variant_of("fscanf.");

	if (IS("float_ulp"))
		bump_results(fmt, r, ap);
	return r;
}

#define ENTRY_S(fn, sym) \
int fn(const char *restrict s, const char *restrict fmt, ...) __asm__(sym); \
int fn(const char *restrict s, const char *restrict fmt, ...) \
{ \
	static vs_fn real; \
	va_list ap, cp; \
	int r; \
	if (real == NULL) real = (vs_fn)real_of("__isoc99_vsscanf", "vsscanf"); \
	va_start(ap, fmt); \
	va_copy(cp, ap); \
	r = real(s, fmt, ap); \
	r = after(r, fmt, cp); \
	va_end(cp); \
	va_end(ap); \
	return r; \
}
#define ENTRY_F(fn, sym) \
int fn(FILE *restrict f, const char *restrict fmt, ...) __asm__(sym); \
int fn(FILE *restrict f, const char *restrict fmt, ...) \
{ \
	static vf_fn real; \
	va_list ap, cp; \
	int r; \
	if (real == NULL) real = (vf_fn)real_of("__isoc99_vfscanf", "vfscanf"); \
	va_start(ap, fmt); \
	va_copy(cp, ap); \
	r = real(f, fmt, ap); \
	r = after(r, fmt, cp); \
	va_end(cp); \
	va_end(ap); \
	return r; \
}
#define ENTRY_IN(fn, sym) \
int fn(const char *restrict fmt, ...) __asm__(sym); \
int fn(const char *restrict fmt, ...) \
{ \
	static vf_fn real; \
	va_list ap, cp; \
	int r; \
	if (real == NULL) real = (vf_fn)real_of("__isoc99_vfscanf", "vfscanf"); \
	va_start(ap, fmt); \
	va_copy(cp, ap); \
	r = real(stdin, fmt, ap); \
	r = after(r, fmt, cp); \
	va_end(cp); \
	va_end(ap); \
	return r; \
}

ENTRY_S(pc_sscanf, "sscanf")
ENTRY_S(pc_sscanf99, "__isoc99_sscanf")
ENTRY_S(pc_sscanf23, "__isoc23_sscanf")
ENTRY_F(pc_fscanf, "fscanf")
ENTRY_F(pc_fscanf99, "__isoc99_fscanf")
ENTRY_F(pc_fscanf23, "__isoc23_fscanf")
ENTRY_IN(pc_scanf, "scanf")
ENTRY_IN(pc_scanf99, "__isoc99_scanf")
ENTRY_IN(pc_scanf23, "__isoc23_scanf")

/*
 * "printf_long_inf": snprintf() spells infinity "infinity" ("INFINITY"
 * for %F and the other upper-case forms) and NaN "nan(1)" ("NAN(1)").
 * fprintf() allows either spelling of infinity and lets a NaN carry an
 * n-char-sequence; this page then requires the scanf() functions to read
 * what fprintf() produces, and strtod()'s subject sequence covers both
 * spellings. Only a whole output that is one such value is respelled.
 */
static int respell(char *out, size_t n, int r)
{
	static const char *from[] = { "inf", "-inf", "INF", "-INF", "nan", "-nan", "NAN", "-NAN" };
	static const char *to[] = { "infinity", "-infinity", "INFINITY", "-INFINITY",
	    "nan(1)", "-nan(1)", "NAN(1)", "-NAN(1)" };
	size_t i;

	if (out == NULL || r < 0 || (size_t)r >= n)
		return r;
	for (i = 0; i < sizeof from / sizeof from[0]; i++)
		if (strcmp(out, from[i]) == 0 && strlen(to[i]) < n) {
			strcpy(out, to[i]);
			return (int)strlen(to[i]);
		}
	return r;
}

int snprintf(char *restrict out, size_t n, const char *restrict fmt, ...)
{
	static vsn_fn real;
	const char *v = variant_of("fscanf.");
	va_list ap;
	int r;

	if (real == NULL)
		real = (vsn_fn)dlsym(RTLD_NEXT, "vsnprintf");
	va_start(ap, fmt);
	r = real(out, n, fmt, ap);
	va_end(ap);
	if (IS("printf_long_inf"))
		r = respell(out, n, r);
	return r;
}
