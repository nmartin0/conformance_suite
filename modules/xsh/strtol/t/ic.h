/*
 * ic.h - the integer string converters behind one signature, and a
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
#include <string.h>

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

typedef R (*ic_f)(const char *, char **, int);

#if defined(IC_L)
static long (*volatile v_1)(const char *restrict, char **restrict, int) = strtol;
static long long (*volatile v_2)(const char *restrict, char **restrict, int) = strtoll;
static R w_1(const char *s, char **e, int b) { return v_1(s, e, b); }
static R w_2(const char *s, char **e, int b) { return v_2(s, e, b); }
#define IC_N 2
#define IC_TABLE { { "strtol", w_1, LONG_MAX, LONG_MIN }, { "strtoll", w_2, LLONG_MAX, LLONG_MIN } }
#elif defined(IC_UL)
static unsigned long (*volatile v_1)(const char *restrict, char **restrict, int) = strtoul;
static unsigned long long (*volatile v_2)(const char *restrict, char **restrict, int) = strtoull;
static R w_1(const char *s, char **e, int b) { return v_1(s, e, b); }
static R w_2(const char *s, char **e, int b) { return v_2(s, e, b); }
#define IC_N 2
#define IC_TABLE { { "strtoul", w_1, ULONG_MAX, 0 }, { "strtoull", w_2, ULLONG_MAX, 0 } }
#elif defined(IC_IMAX)
static intmax_t (*volatile v_1)(const char *restrict, char **restrict, int) = strtoimax;
static R w_1(const char *s, char **e, int b) { return v_1(s, e, b); }
#define IC_N 1
#define IC_TABLE { { "strtoimax", w_1, INTMAX_MAX, INTMAX_MIN } }
#else
static uintmax_t (*volatile v_1)(const char *restrict, char **restrict, int) = strtoumax;
static R w_1(const char *s, char **e, int b) { return v_1(s, e, b); }
#define IC_N 1
#define IC_TABLE { { "strtoumax", w_1, UINTMAX_MAX, 0 } }
#endif

struct ic_fn {
	const char *name;
	ic_f f;
	R max;
	R min;
};

static const struct ic_fn ic_fns[IC_N] = IC_TABLE;

/* A conversion expected to succeed: value and bytes consumed (-1: all). */
struct ic_case {
	const char *in;
	int base;
	R val;
	int end;
};

#define IC_X(v) ((unsigned long long)(v))

static void ic_run(const struct ic_case *c, int n)
{
	int j, k, err;
	char *e;
	R r;

	for (j = 0; j < IC_N; j++)
		for (k = 0; k < n; k++) {
			e = NULL;
			errno = 0;
			r = ic_fns[j].f(c[k].in, &e, c[k].base);
			err = errno;
			pc_check(r == c[k].val, "%s(\"%s\", base %d) returned 0x%llx, expected 0x%llx",
				ic_fns[j].name, c[k].in, c[k].base, IC_X(r), IC_X(c[k].val));
			pc_check(e == c[k].in + (c[k].end < 0 ? strlen(c[k].in) : (size_t)c[k].end),
				"%s(\"%s\", base %d) left the end pointer at offset %ld",
				ic_fns[j].name, c[k].in, c[k].base, (long)(e - c[k].in));
			pc_check(err == 0, "%s(\"%s\", base %d) changed errno to %s",
				ic_fns[j].name, c[k].in, c[k].base, pc_errname(err));
		}
}

#endif
