/*
 * A type-generic macro called with real arguments picks float, double or
 *   long double by the arguments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tgmath.h.002
 */
#include <tgmath.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)
#define T(expr, type) pc_check(IS_T(expr, type), #expr " does not have type " #type)

int main(void)
{
	float f = 1.5f;
	double d = 1.5;
	long double ld = 1.5L;
	int n = 1, q;

	pc_begin("XBD.tgmath.h.002");
	pc_test();
	T(sin(f), float);
	T(sin(d), double);
	T(sin(ld), long double);
	T(sin(n), double);
	T(sin((short)n), double);
	T(acosh(f), float);
	T(atan(ld), long double);
	T(exp(n), double);
	T(cos(d), double);
	T(atan2(f, f), float);
	T(atan2(f, d), double);
	T(atan2(f, ld), long double);
	T(atan2(n, f), double);
	T(atan2(n, n), double);
	T(pow(f, f), float);
	T(pow(d, f), double);
	T(pow(ld, n), long double);
	T(fma(f, f, f), float);
	T(fma(f, f, ld), long double);
	T(fma(f, d, f), double);
	T(copysign(n, ld), long double);
	T(copysign(f, f), float);
	T(nexttoward(f, ld), float);
	T(nexttoward(d, ld), double);
	T(nexttoward(ld, f), long double);
	T(nextafter(d, f), double);
	T(nextafter(f, f), float);
	T(remainder(n, n), double);
	T(remainder(f, ld), long double);
	T(fabs(f), float);
	T(fabs(d), double);
	T(fabs(ld), long double);
	T(fabs(n), double);
	T(ceil(f), float);
	T(floor(ld), long double);
	T(trunc(n), double);
	T(round(f), float);
	T(fmax(f, d), double);
	T(fmin(f, f), float);
	T(hypot(f, f), float);
	T(cbrt(ld), long double);
	T(exp2(f), float);
	T(log2(ld), long double);
	T(lgamma(f), float);
	T(erf(d), double);
	T(ldexp(f, n), float);
	T(scalbn(ld, n), long double);
	T(scalbln(f, 2L), float);
	T(frexp(f, &q), float);
	T(frexp(d, &q), double);
	T(remquo(f, f, &q), float);
	T(remquo(ld, d, &q), long double);
	T(ilogb(f), int);
	T(lround(f), long);
	T(llround(ld), long long);
	T(lrint(d), long);
	T(llrint(f), long long);
	T(nearbyint(f), float);
	T(rint(ld), long double);
	T(tgamma(n), double);
	(void)f; (void)d; (void)ld; (void)n; (void)q;
	pc_pass();
}
