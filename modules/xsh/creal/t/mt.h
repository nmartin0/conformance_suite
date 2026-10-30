/*
 * mt.h - helpers shared by the tests of one complex-number module.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Arguments and results of the calls under test go through volatile
 * objects and are built with CMPLX(), so that nothing is folded at
 * translation time. Cases are written once, as long double values, and run
 * for double, float and long double (type 0, 1 and 2). Components of a
 * result are read through the array layout of a complex type, not through
 * creal() and cimag(), so that those functions can themselves be tested.
 * The IEC 60559 special values on these pages carry the tag for the
 * Complex Floating-Point option; a test of them is NOTINUSE unless the C
 * implementation claims it. Exception flags are judged only when
 * (math_errhandling & MATH_ERREXCEPT) is non-zero. No errors are defined on
 * these pages, so errno is not looked at.
 */
#ifndef MT_H
#define MT_H

#include "pc.h"
#include <complex.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <math.h>

/*
 * CMPLX() is required, but a missing definition must not hide every test
 * that only needs a way to build an operand (the CMPLX modules themselves
 * report the absence).  A complex object has the same representation as an
 * array of its two parts, so the operand is assembled that way.
 */
#include <string.h>
#ifndef CMPLX
static double _Complex mt_cmplx_d(double re, double im)
{ double a[2]; double _Complex z; a[0] = re; a[1] = im; memcpy(&z, a, sizeof z); return z; }
#define CMPLX(re, im) mt_cmplx_d((re), (im))
#endif
#ifndef CMPLXF
static float _Complex mt_cmplx_f(float re, float im)
{ float a[2]; float _Complex z; a[0] = re; a[1] = im; memcpy(&z, a, sizeof z); return z; }
#define CMPLXF(re, im) mt_cmplx_f((re), (im))
#endif
#ifndef CMPLXL
static long double _Complex mt_cmplx_l(long double re, long double im)
{ long double a[2]; long double _Complex z; a[0] = re; a[1] = im; memcpy(&z, a, sizeof z); return z; }
#define CMPLXL(re, im) mt_cmplx_l((re), (im))
#endif

#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF

#if defined(__STDC_IEC_559_COMPLEX__) || defined(__STDC_IEC_60559_COMPLEX__)
#define MT_MXC 1
#else
#define MT_MXC 0
#endif
#define MT_NEED_MXC() do { if (!MT_MXC) pc_notinuse("the IEC 60559 Complex Floating-Point option is not claimed"); } while (0)

#ifndef INFINITY
#define INFINITY 0.0f
#define MT_NO_INF 1
#else
#define MT_NO_INF 0
#endif
#ifndef NAN
#define NAN 0.0f
#define MT_NO_NAN 1
#else
#define MT_NO_NAN 0
#endif
#ifndef FE_INVALID
#define FE_INVALID 0
#endif
#ifndef FE_DIVBYZERO
#define FE_DIVBYZERO 0
#endif

static int mt_fl;		/* exception flags saved right after a call */

#define MT_START() (void)feclearexcept(FE_ALL_EXCEPT)
#define MT_SAVE() do { mt_fl = fetestexcept(FE_ALL_EXCEPT); } while (0)

/* Expectation kinds. */
enum { K_ANY, K_VAL, K_P0, K_N0, K_Z, K_PINF, K_NINF, K_INF, K_NAN, K_Q, K_QABS };
struct ex { int k; long double v; };
/* Row flags: exceptions that must be raised. */
enum { FL_INV = 1, FL_DIV = 2 };
/* A case: input real and imaginary part, expected real and imaginary part. */
struct row { struct ex ir, ii, er, ei; int fl; };

#define X_ANY	{ K_ANY, 0 }
#define X_P0	{ K_P0, 0 }
#define X_N0	{ K_N0, 0 }
#define X_Z	{ K_Z, 0 }
#define X_PINF	{ K_PINF, 0 }
#define X_NINF	{ K_NINF, 0 }
#define X_INF	{ K_INF, 0 }
#define X_NAN	{ K_NAN, 0 }
#define X_V(x)	{ K_VAL, (x) }
/* A multiple of pi/4 (4 is pi, 2 is pi/2), signed; QA matches either sign. */
#define X_Q(n)	{ K_Q, (n) }
#define X_QA(n)	{ K_QABS, (n) }

typedef void (*ct_fn)(int ty, long double re, long double im, long double *ore, long double *oim);

/*
 * Wrappers. N is the function name; N##f and N##l are the other two types.
 * CC: complex to complex. CR: complex to real (the imaginary result is 0).
 */

#define CT_WRAP_CC(N) \
static void w_##N(int ty, long double re, long double im, long double *ore, long double *oim) \
{ \
	if (ty == 0) { \
		volatile double complex z; volatile double complex r; const volatile double *p; \
		z = CMPLX((double)re, (double)im); \
		MT_START(); r = N(z); MT_SAVE(); \
		p = (const volatile double *)&r; *ore = p[0]; *oim = p[1]; \
	} else if (ty == 1) { \
		volatile float complex z; volatile float complex r; const volatile float *p; \
		z = CMPLXF((float)re, (float)im); \
		MT_START(); r = N##f(z); MT_SAVE(); \
		p = (const volatile float *)&r; *ore = p[0]; *oim = p[1]; \
	} else { \
		volatile long double complex z; volatile long double complex r; const volatile long double *p; \
		z = CMPLXL(re, im); \
		MT_START(); r = N##l(z); MT_SAVE(); \
		p = (const volatile long double *)&r; *ore = p[0]; *oim = p[1]; \
	} \
}

#define CT_WRAP_CR(N) \
static void w_##N(int ty, long double re, long double im, long double *ore, long double *oim) \
{ \
	if (ty == 0) { \
		volatile double complex z; volatile double r; \
		z = CMPLX((double)re, (double)im); \
		MT_START(); r = N(z); MT_SAVE(); \
		*ore = r; *oim = 0; \
	} else if (ty == 1) { \
		volatile float complex z; volatile float r; \
		z = CMPLXF((float)re, (float)im); \
		MT_START(); r = N##f(z); MT_SAVE(); \
		*ore = r; *oim = 0; \
	} else { \
		volatile long double complex z; volatile long double r; \
		z = CMPLXL(re, im); \
		MT_START(); r = N##l(z); MT_SAVE(); \
		*ore = r; *oim = 0; \
	} \
}

static const char *const ct_tyname[3] = { "double", "float", "long double" };

/* Whether the type has a negative zero. */
static int ct_hasnz(int ty)
{
	volatile float f = -0.0f;
	volatile double d = -0.0;
	volatile long double l = -0.0L;

	return ty == 0 ? signbit(d) != 0 : ty == 1 ? signbit(f) != 0 : signbit(l) != 0;
}

/* pi/4 times n (n from 1 to 4, sign kept), as the type represents it: the decimal value rounded to the type. */
static long double ct_pi4(int ty, int n)
{
	volatile float f = 0;
	volatile double d = 0;
	volatile long double l = 0;
	long double a;

	switch (n < 0 ? -n : n) {
	case 1:
		f = 0.7853981633974483096156608458198757210493f;
		d = 0.7853981633974483096156608458198757210493;
		l = 0.7853981633974483096156608458198757210493L;
		break;
	case 2:
		f = 1.570796326794896619231321691639751442099f;
		d = 1.570796326794896619231321691639751442099;
		l = 1.570796326794896619231321691639751442099L;
		break;
	case 3:
		f = 2.356194490192344928846982537459627163148f;
		d = 2.356194490192344928846982537459627163148;
		l = 2.356194490192344928846982537459627163148L;
		break;
	default:
		f = 3.141592653589793238462643383279502884197f;
		d = 3.141592653589793238462643383279502884197;
		l = 3.141592653589793238462643383279502884197L;
		break;
	}
	a = ty == 0 ? (long double)d : ty == 1 ? (long double)f : l;
	return n < 0 ? -a : a;
}

static int ct_usable(struct ex e)
{
	if ((e.k == K_PINF || e.k == K_NINF) && MT_NO_INF)
		return 0;
	if (e.k == K_NAN && MT_NO_NAN)
		return 0;
	return 1;
}

static long double ct_in(struct ex e)
{
	switch (e.k) {
	case K_P0: return 0.0L;
	case K_N0: return -0.0L;
	case K_PINF: return (long double)INFINITY;
	case K_NINF: return -(long double)INFINITY;
	case K_NAN: return (long double)NAN;
	default: return e.v;
	}
}

/* The expectation for the result of multiplying by -i: (a + ib) becomes (b - ia). */
static struct ex ct_neg(struct ex e)
{
	switch (e.k) {
	case K_VAL: case K_Q: e.v = -e.v; break;
	case K_P0: e.k = K_N0; break;
	case K_N0: e.k = K_P0; break;
	case K_PINF: e.k = K_NINF; break;
	case K_NINF: e.k = K_PINF; break;
	default: break;
	}
	return e;
}

static int ct_match(int ty, int nz, struct ex e, long double got)
{
	int k = e.k;

	if (!nz && (k == K_P0 || k == K_N0))
		k = K_Z;
	switch (k) {
	case K_ANY: return 1;
	case K_VAL: return got == e.v;
	case K_P0: return got == 0 && !signbit(got);
	case K_N0: return got == 0 && signbit(got) != 0;
	case K_Z: return got == 0;
	case K_PINF: return isinf(got) && !signbit(got);
	case K_NINF: return isinf(got) && signbit(got) != 0;
	case K_INF: return isinf(got) != 0;
	case K_NAN: return isnan(got) != 0;
	case K_Q: return got == ct_pi4(ty, (int)e.v);
	case K_QABS: return got == ct_pi4(ty, (int)e.v) || got == -ct_pi4(ty, (int)e.v);
	}
	return 0;
}

/*
 * Runs the rows for each of the three types. Mode 0: the function is called
 * on the input as written. Mode 1: the input is the product i*z of the
 * page's notation, so z = (Im, -Re) is passed, and the page's result is
 * -i times the expectation, (Im, -Re) of it. Mode 2: z as in mode 1, the
 * expectation is used as written. Multiplication by i is taken as the exact
 * exchange of the parts with one sign change (XBD complex arithmetic, Annex G).
 */
static void ct_run(const char *name, ct_fn fn, const struct row *rows, int n, int mode, int isreal)
{
	int ty, i;

	for (ty = 0; ty < 3; ty++) {
		int nz = ct_hasnz(ty);

		for (i = 0; i < n; i++) {
			const struct row *r = &rows[i];
			struct ex er = r->er, ei = r->ei;
			long double a, b, zr, zi, gr = 0, gi = 0;

			if (!ct_usable(r->ir) || !ct_usable(r->ii))
				continue;
			if (!nz && (r->ir.k == K_N0 || r->ii.k == K_N0))
				continue;
			a = ct_in(r->ir);
			b = ct_in(r->ii);
			if (mode == 0) {
				zr = a;
				zi = b;
			} else {
				zr = b;
				zi = -a;
				if (mode == 1) {
					struct ex t = er;

					er = ei;
					ei = ct_neg(t);
				}
			}
			fn(ty, zr, zi, &gr, &gi);
			pc_check(ct_match(ty, nz, er, gr) && (isreal || ct_match(ty, nz, ei, gi)),
				"%s [%s] (%Lg, %Lg) returned (%Lg, %Lg) in case %d, which does not match",
				name, ct_tyname[ty], zr, zi, gr, gi, i);
			if (math_errhandling & MATH_ERREXCEPT) {
				if ((r->fl & FL_INV) && FE_INVALID)
					pc_check((mt_fl & FE_INVALID) != 0, "%s [%s] (%Lg, %Lg): invalid exception not raised (case %d)", name, ct_tyname[ty], zr, zi, i);
				if ((r->fl & FL_DIV) && FE_DIVBYZERO)
					pc_check((mt_fl & FE_DIVBYZERO) != 0, "%s [%s] (%Lg, %Lg): divide-by-zero exception not raised (case %d)", name, ct_tyname[ty], zr, zi, i);
			}
		}
	}
}

/* Same value: equal and with the same zero sign, or both NaN. */
static int ct_same(long double x, long double y)
{
	if (isnan(x) || isnan(y))
		return isnan(x) && isnan(y);
	return x == y && (x != 0 || (signbit(x) != 0) == (signbit(y) != 0));
}

/* Points used by the symmetry tests: no NaN. */
static const struct ex ct_pts[][2] = {
	{ X_V(0.5L), X_V(0.25L) }, { X_V(2.0L), X_V(1.5L) }, { X_V(-1.5L), X_V(0.75L) },
	{ X_P0, X_P0 }, { X_N0, X_P0 }, { X_P0, X_V(1.0L) }, { X_V(1.0L), X_P0 },
	{ X_V(2.0L), X_P0 }, { X_V(-2.0L), X_P0 }, { X_V(0.5L), X_P0 }, { X_V(-0.5L), X_P0 },
	{ X_P0, X_PINF }, { X_V(1.0L), X_PINF }, { X_PINF, X_V(1.0L) }, { X_PINF, X_PINF },
	{ X_NINF, X_V(1.0L) }, { X_PINF, X_P0 }, { X_NINF, X_PINF }, { X_NINF, X_P0 },
};
/* Extra points with a NaN real part (used by the conjugate test only). */
static const struct ex ct_pts_nan[][2] = {
	{ X_NAN, X_V(1.0L) }, { X_NAN, X_P0 }, { X_NAN, X_PINF },
};

enum { SYM_CONJ, SYM_ODD, SYM_EVEN };

static void ct_sym_point(const char *name, ct_fn fn, int kind, int ty, int nz, struct ex px, struct ex py)
{
	long double x, y, x2, y2, r1x, r1y, r2x, r2y, e2x, e2y;

	if (!ct_usable(px) || !ct_usable(py))
		return;
	x = ct_in(px);
	y = ct_in(py);
	if (kind == SYM_CONJ) {
		x2 = x;
		y2 = -y;
	} else {
		x2 = -x;
		y2 = -y;
	}
	if (!nz && ((x == 0 && signbit(x)) || (y == 0 && signbit(y)) || (x2 == 0 && signbit(x2)) || (y2 == 0 && signbit(y2))))
		return;
	fn(ty, x, y, &r1x, &r1y);
	fn(ty, x2, y2, &r2x, &r2y);
	if (kind == SYM_CONJ) {
		e2x = r1x;
		e2y = -r1y;
	} else if (kind == SYM_ODD) {
		e2x = -r1x;
		e2y = -r1y;
	} else {
		e2x = r1x;
		e2y = r1y;
	}
	pc_check(ct_same(r2x, e2x) && ct_same(r2y, e2y),
		"%s [%s]: at (%Lg, %Lg) the result is (%Lg, %Lg), at (%Lg, %Lg) it is (%Lg, %Lg), expected (%Lg, %Lg)",
		name, ct_tyname[ty], x, y, r1x, r1y, x2, y2, r2x, r2y, e2x, e2y);
}

static void ct_sym(const char *name, ct_fn fn, int kind)
{
	int ty, i;

	for (ty = 0; ty < 3; ty++) {
		int nz = ct_hasnz(ty);

		for (i = 0; i < (int)(sizeof ct_pts / sizeof ct_pts[0]); i++)
			ct_sym_point(name, fn, kind, ty, nz, ct_pts[i][0], ct_pts[i][1]);
		if (kind == SYM_CONJ)
			for (i = 0; i < (int)(sizeof ct_pts_nan / sizeof ct_pts_nan[0]); i++)
				ct_sym_point(name, fn, kind, ty, nz, ct_pts_nan[i][0], ct_pts_nan[i][1]);
	}
}

#endif
