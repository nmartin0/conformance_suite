/*
 * ic.h - the integer wide-string converters behind one signature, and a
 * table driven checker. IC_L, IC_UL, IC_IMAX or IC_UMAX picks the family.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef IC_H
#define IC_H

#include "pc.h"
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <wchar.h>

#if defined(IC_L)
typedef long long R;
#define IC_SIGNED 1
#elif defined(IC_UL)
typedef unsigned long long R;
#define IC_SIGNED 0
#elif defined(IC_IMAX)
typedef intmax_t R;
#define IC_SIGNED 1
#elif defined(IC_UMAX)
typedef uintmax_t R;
#define IC_SIGNED 0
#else
#error "pick a family"
#endif

typedef R (*ic_f)(const wchar_t *, wchar_t **, int);

#if defined(IC_L)
static R w_1(const wchar_t *s, wchar_t **e, int b) { return wcstol(s, e, b); }
static R w_2(const wchar_t *s, wchar_t **e, int b) { return wcstoll(s, e, b); }
#define IC_N 2
#define IC_TABLE { { "wcstol", w_1, LONG_MAX, LONG_MIN }, { "wcstoll", w_2, LLONG_MAX, LLONG_MIN } }
#elif defined(IC_UL)
static R w_1(const wchar_t *s, wchar_t **e, int b) { return wcstoul(s, e, b); }
static R w_2(const wchar_t *s, wchar_t **e, int b) { return wcstoull(s, e, b); }
#define IC_N 2
#define IC_TABLE { { "wcstoul", w_1, ULONG_MAX, 0 }, { "wcstoull", w_2, ULLONG_MAX, 0 } }
#elif defined(IC_IMAX)
static R w_1(const wchar_t *s, wchar_t **e, int b) { return wcstoimax(s, e, b); }
#define IC_N 1
#define IC_TABLE { { "wcstoimax", w_1, INTMAX_MAX, INTMAX_MIN } }
#else
static R w_1(const wchar_t *s, wchar_t **e, int b) { return wcstoumax(s, e, b); }
#define IC_N 1
#define IC_TABLE { { "wcstoumax", w_1, UINTMAX_MAX, 0 } }
#endif

struct ic_fn {
	const char *name;
	ic_f f;
	R max;
	R min;
};

static const struct ic_fn ic_fns[IC_N] = IC_TABLE;

/* A conversion expected to succeed: value and wide characters consumed (-1: all). */
struct ic_case {
	const wchar_t *in;
	int base;
	R val;
	int end;
};

#define IC_X(v) ((unsigned long long)(v))

static void ic_run(const struct ic_case *c, int n)
{
	int j, k, err;
	wchar_t *e;
	R r;

	for (j = 0; j < IC_N; j++)
		for (k = 0; k < n; k++) {
			e = NULL;
			errno = 0;
			r = ic_fns[j].f(c[k].in, &e, c[k].base);
			err = errno;
			pc_check(r == c[k].val, "%s(\"%ls\", base %d) returned 0x%llx, expected 0x%llx",
				ic_fns[j].name, c[k].in, c[k].base, IC_X(r), IC_X(c[k].val));
			pc_check(e == c[k].in + (c[k].end < 0 ? wcslen(c[k].in) : (size_t)c[k].end),
				"%s(\"%ls\", base %d) left the end pointer at offset %ld",
				ic_fns[j].name, c[k].in, c[k].base, (long)(e - c[k].in));
			pc_check(err == 0, "%s(\"%ls\", base %d) changed errno to %s",
				ic_fns[j].name, c[k].in, c[k].base, pc_errname(err));
		}
}

#endif
