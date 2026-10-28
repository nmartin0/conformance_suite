/*
 * <math.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.016
 *
 * Only <math.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <math.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.math.h.016");
	pc_test();
	if (!IS_T(&(acos), double (*)(double)))
		pc_fail("acos() is not declared as double acos(double)");
	if (IS_T(&(acos), double (*)(double, int)))
		pc_fail("acos() is declared without a prototype");
	if (!IS_T(&(acosf), float (*)(float)))
		pc_fail("acosf() is not declared as float acosf(float)");
	if (IS_T(&(acosf), float (*)(float, int)))
		pc_fail("acosf() is declared without a prototype");
	if (!IS_T(&(acosh), double (*)(double)))
		pc_fail("acosh() is not declared as double acosh(double)");
	if (IS_T(&(acosh), double (*)(double, int)))
		pc_fail("acosh() is declared without a prototype");
	if (!IS_T(&(acoshf), float (*)(float)))
		pc_fail("acoshf() is not declared as float acoshf(float)");
	if (IS_T(&(acoshf), float (*)(float, int)))
		pc_fail("acoshf() is declared without a prototype");
	if (!IS_T(&(acoshl), long double (*)(long double)))
		pc_fail("acoshl() is not declared as long double acoshl(long double)");
	if (IS_T(&(acoshl), long double (*)(long double, int)))
		pc_fail("acoshl() is declared without a prototype");
	if (!IS_T(&(acosl), long double (*)(long double)))
		pc_fail("acosl() is not declared as long double acosl(long double)");
	if (IS_T(&(acosl), long double (*)(long double, int)))
		pc_fail("acosl() is declared without a prototype");
	if (!IS_T(&(asin), double (*)(double)))
		pc_fail("asin() is not declared as double asin(double)");
	if (IS_T(&(asin), double (*)(double, int)))
		pc_fail("asin() is declared without a prototype");
	if (!IS_T(&(asinf), float (*)(float)))
		pc_fail("asinf() is not declared as float asinf(float)");
	if (IS_T(&(asinf), float (*)(float, int)))
		pc_fail("asinf() is declared without a prototype");
	if (!IS_T(&(asinh), double (*)(double)))
		pc_fail("asinh() is not declared as double asinh(double)");
	if (IS_T(&(asinh), double (*)(double, int)))
		pc_fail("asinh() is declared without a prototype");
	if (!IS_T(&(asinhf), float (*)(float)))
		pc_fail("asinhf() is not declared as float asinhf(float)");
	if (IS_T(&(asinhf), float (*)(float, int)))
		pc_fail("asinhf() is declared without a prototype");
	if (!IS_T(&(asinhl), long double (*)(long double)))
		pc_fail("asinhl() is not declared as long double asinhl(long double)");
	if (IS_T(&(asinhl), long double (*)(long double, int)))
		pc_fail("asinhl() is declared without a prototype");
	if (!IS_T(&(asinl), long double (*)(long double)))
		pc_fail("asinl() is not declared as long double asinl(long double)");
	if (IS_T(&(asinl), long double (*)(long double, int)))
		pc_fail("asinl() is declared without a prototype");
	if (!IS_T(&(atan), double (*)(double)))
		pc_fail("atan() is not declared as double atan(double)");
	if (IS_T(&(atan), double (*)(double, int)))
		pc_fail("atan() is declared without a prototype");
	if (!IS_T(&(atan2), double (*)(double, double)))
		pc_fail("atan2() is not declared as double atan2(double, double)");
	if (IS_T(&(atan2), double (*)(double, double, int)))
		pc_fail("atan2() is declared without a prototype");
	if (!IS_T(&(atan2f), float (*)(float, float)))
		pc_fail("atan2f() is not declared as float atan2f(float, float)");
	if (IS_T(&(atan2f), float (*)(float, float, int)))
		pc_fail("atan2f() is declared without a prototype");
	if (!IS_T(&(atan2l), long double (*)(long double, long double)))
		pc_fail("atan2l() is not declared as long double atan2l(long double, long double)");
	if (IS_T(&(atan2l), long double (*)(long double, long double, int)))
		pc_fail("atan2l() is declared without a prototype");
	if (!IS_T(&(atanf), float (*)(float)))
		pc_fail("atanf() is not declared as float atanf(float)");
	if (IS_T(&(atanf), float (*)(float, int)))
		pc_fail("atanf() is declared without a prototype");
	if (!IS_T(&(atanh), double (*)(double)))
		pc_fail("atanh() is not declared as double atanh(double)");
	if (IS_T(&(atanh), double (*)(double, int)))
		pc_fail("atanh() is declared without a prototype");
	if (!IS_T(&(atanhf), float (*)(float)))
		pc_fail("atanhf() is not declared as float atanhf(float)");
	if (IS_T(&(atanhf), float (*)(float, int)))
		pc_fail("atanhf() is declared without a prototype");
	if (!IS_T(&(atanhl), long double (*)(long double)))
		pc_fail("atanhl() is not declared as long double atanhl(long double)");
	if (IS_T(&(atanhl), long double (*)(long double, int)))
		pc_fail("atanhl() is declared without a prototype");
	if (!IS_T(&(atanl), long double (*)(long double)))
		pc_fail("atanl() is not declared as long double atanl(long double)");
	if (IS_T(&(atanl), long double (*)(long double, int)))
		pc_fail("atanl() is declared without a prototype");
	if (!IS_T(&(cbrt), double (*)(double)))
		pc_fail("cbrt() is not declared as double cbrt(double)");
	if (IS_T(&(cbrt), double (*)(double, int)))
		pc_fail("cbrt() is declared without a prototype");
	if (!IS_T(&(cbrtf), float (*)(float)))
		pc_fail("cbrtf() is not declared as float cbrtf(float)");
	if (IS_T(&(cbrtf), float (*)(float, int)))
		pc_fail("cbrtf() is declared without a prototype");
	if (!IS_T(&(cbrtl), long double (*)(long double)))
		pc_fail("cbrtl() is not declared as long double cbrtl(long double)");
	if (IS_T(&(cbrtl), long double (*)(long double, int)))
		pc_fail("cbrtl() is declared without a prototype");
	if (!IS_T(&(ceil), double (*)(double)))
		pc_fail("ceil() is not declared as double ceil(double)");
	if (IS_T(&(ceil), double (*)(double, int)))
		pc_fail("ceil() is declared without a prototype");
	if (!IS_T(&(ceilf), float (*)(float)))
		pc_fail("ceilf() is not declared as float ceilf(float)");
	if (IS_T(&(ceilf), float (*)(float, int)))
		pc_fail("ceilf() is declared without a prototype");
	if (!IS_T(&(ceill), long double (*)(long double)))
		pc_fail("ceill() is not declared as long double ceill(long double)");
	if (IS_T(&(ceill), long double (*)(long double, int)))
		pc_fail("ceill() is declared without a prototype");
	if (!IS_T(&(copysign), double (*)(double, double)))
		pc_fail("copysign() is not declared as double copysign(double, double)");
	if (IS_T(&(copysign), double (*)(double, double, int)))
		pc_fail("copysign() is declared without a prototype");
	if (!IS_T(&(copysignf), float (*)(float, float)))
		pc_fail("copysignf() is not declared as float copysignf(float, float)");
	if (IS_T(&(copysignf), float (*)(float, float, int)))
		pc_fail("copysignf() is declared without a prototype");
	if (!IS_T(&(copysignl), long double (*)(long double, long double)))
		pc_fail("copysignl() is not declared as long double copysignl(long double, long double)");
	if (IS_T(&(copysignl), long double (*)(long double, long double, int)))
		pc_fail("copysignl() is declared without a prototype");
	if (!IS_T(&(cos), double (*)(double)))
		pc_fail("cos() is not declared as double cos(double)");
	if (IS_T(&(cos), double (*)(double, int)))
		pc_fail("cos() is declared without a prototype");
	if (!IS_T(&(cosf), float (*)(float)))
		pc_fail("cosf() is not declared as float cosf(float)");
	if (IS_T(&(cosf), float (*)(float, int)))
		pc_fail("cosf() is declared without a prototype");
	if (!IS_T(&(cosh), double (*)(double)))
		pc_fail("cosh() is not declared as double cosh(double)");
	if (IS_T(&(cosh), double (*)(double, int)))
		pc_fail("cosh() is declared without a prototype");
	if (!IS_T(&(coshf), float (*)(float)))
		pc_fail("coshf() is not declared as float coshf(float)");
	if (IS_T(&(coshf), float (*)(float, int)))
		pc_fail("coshf() is declared without a prototype");
	if (!IS_T(&(coshl), long double (*)(long double)))
		pc_fail("coshl() is not declared as long double coshl(long double)");
	if (IS_T(&(coshl), long double (*)(long double, int)))
		pc_fail("coshl() is declared without a prototype");
	if (!IS_T(&(cosl), long double (*)(long double)))
		pc_fail("cosl() is not declared as long double cosl(long double)");
	if (IS_T(&(cosl), long double (*)(long double, int)))
		pc_fail("cosl() is declared without a prototype");
	if (!IS_T(&(erf), double (*)(double)))
		pc_fail("erf() is not declared as double erf(double)");
	if (IS_T(&(erf), double (*)(double, int)))
		pc_fail("erf() is declared without a prototype");
	if (!IS_T(&(erfc), double (*)(double)))
		pc_fail("erfc() is not declared as double erfc(double)");
	if (IS_T(&(erfc), double (*)(double, int)))
		pc_fail("erfc() is declared without a prototype");
	if (!IS_T(&(erfcf), float (*)(float)))
		pc_fail("erfcf() is not declared as float erfcf(float)");
	if (IS_T(&(erfcf), float (*)(float, int)))
		pc_fail("erfcf() is declared without a prototype");
	if (!IS_T(&(erfcl), long double (*)(long double)))
		pc_fail("erfcl() is not declared as long double erfcl(long double)");
	if (IS_T(&(erfcl), long double (*)(long double, int)))
		pc_fail("erfcl() is declared without a prototype");
	if (!IS_T(&(erff), float (*)(float)))
		pc_fail("erff() is not declared as float erff(float)");
	if (IS_T(&(erff), float (*)(float, int)))
		pc_fail("erff() is declared without a prototype");
	if (!IS_T(&(erfl), long double (*)(long double)))
		pc_fail("erfl() is not declared as long double erfl(long double)");
	if (IS_T(&(erfl), long double (*)(long double, int)))
		pc_fail("erfl() is declared without a prototype");
	if (!IS_T(&(exp), double (*)(double)))
		pc_fail("exp() is not declared as double exp(double)");
	if (IS_T(&(exp), double (*)(double, int)))
		pc_fail("exp() is declared without a prototype");
	if (!IS_T(&(exp2), double (*)(double)))
		pc_fail("exp2() is not declared as double exp2(double)");
	if (IS_T(&(exp2), double (*)(double, int)))
		pc_fail("exp2() is declared without a prototype");
	if (!IS_T(&(exp2f), float (*)(float)))
		pc_fail("exp2f() is not declared as float exp2f(float)");
	if (IS_T(&(exp2f), float (*)(float, int)))
		pc_fail("exp2f() is declared without a prototype");
	if (!IS_T(&(exp2l), long double (*)(long double)))
		pc_fail("exp2l() is not declared as long double exp2l(long double)");
	if (IS_T(&(exp2l), long double (*)(long double, int)))
		pc_fail("exp2l() is declared without a prototype");
	if (!IS_T(&(expf), float (*)(float)))
		pc_fail("expf() is not declared as float expf(float)");
	if (IS_T(&(expf), float (*)(float, int)))
		pc_fail("expf() is declared without a prototype");
	if (!IS_T(&(expl), long double (*)(long double)))
		pc_fail("expl() is not declared as long double expl(long double)");
	if (IS_T(&(expl), long double (*)(long double, int)))
		pc_fail("expl() is declared without a prototype");
	if (!IS_T(&(expm1), double (*)(double)))
		pc_fail("expm1() is not declared as double expm1(double)");
	if (IS_T(&(expm1), double (*)(double, int)))
		pc_fail("expm1() is declared without a prototype");
	if (!IS_T(&(expm1f), float (*)(float)))
		pc_fail("expm1f() is not declared as float expm1f(float)");
	if (IS_T(&(expm1f), float (*)(float, int)))
		pc_fail("expm1f() is declared without a prototype");
	if (!IS_T(&(expm1l), long double (*)(long double)))
		pc_fail("expm1l() is not declared as long double expm1l(long double)");
	if (IS_T(&(expm1l), long double (*)(long double, int)))
		pc_fail("expm1l() is declared without a prototype");
	if (!IS_T(&(fabs), double (*)(double)))
		pc_fail("fabs() is not declared as double fabs(double)");
	if (IS_T(&(fabs), double (*)(double, int)))
		pc_fail("fabs() is declared without a prototype");
	if (!IS_T(&(fabsf), float (*)(float)))
		pc_fail("fabsf() is not declared as float fabsf(float)");
	if (IS_T(&(fabsf), float (*)(float, int)))
		pc_fail("fabsf() is declared without a prototype");
	if (!IS_T(&(fabsl), long double (*)(long double)))
		pc_fail("fabsl() is not declared as long double fabsl(long double)");
	if (IS_T(&(fabsl), long double (*)(long double, int)))
		pc_fail("fabsl() is declared without a prototype");
	if (!IS_T(&(fdim), double (*)(double, double)))
		pc_fail("fdim() is not declared as double fdim(double, double)");
	if (IS_T(&(fdim), double (*)(double, double, int)))
		pc_fail("fdim() is declared without a prototype");
	if (!IS_T(&(fdimf), float (*)(float, float)))
		pc_fail("fdimf() is not declared as float fdimf(float, float)");
	if (IS_T(&(fdimf), float (*)(float, float, int)))
		pc_fail("fdimf() is declared without a prototype");
	if (!IS_T(&(fdiml), long double (*)(long double, long double)))
		pc_fail("fdiml() is not declared as long double fdiml(long double, long double)");
	if (IS_T(&(fdiml), long double (*)(long double, long double, int)))
		pc_fail("fdiml() is declared without a prototype");
	if (!IS_T(&(floor), double (*)(double)))
		pc_fail("floor() is not declared as double floor(double)");
	if (IS_T(&(floor), double (*)(double, int)))
		pc_fail("floor() is declared without a prototype");
	if (!IS_T(&(floorf), float (*)(float)))
		pc_fail("floorf() is not declared as float floorf(float)");
	if (IS_T(&(floorf), float (*)(float, int)))
		pc_fail("floorf() is declared without a prototype");
	if (!IS_T(&(floorl), long double (*)(long double)))
		pc_fail("floorl() is not declared as long double floorl(long double)");
	if (IS_T(&(floorl), long double (*)(long double, int)))
		pc_fail("floorl() is declared without a prototype");
	if (!IS_T(&(fma), double (*)(double, double, double)))
		pc_fail("fma() is not declared as double fma(double, double, double)");
	if (IS_T(&(fma), double (*)(double, double, double, int)))
		pc_fail("fma() is declared without a prototype");
	if (!IS_T(&(fmaf), float (*)(float, float, float)))
		pc_fail("fmaf() is not declared as float fmaf(float, float, float)");
	if (IS_T(&(fmaf), float (*)(float, float, float, int)))
		pc_fail("fmaf() is declared without a prototype");
	if (!IS_T(&(fmal), long double (*)(long double, long double, long double)))
		pc_fail("fmal() is not declared as long double fmal(long double, long double, long double)");
	if (IS_T(&(fmal), long double (*)(long double, long double, long double, int)))
		pc_fail("fmal() is declared without a prototype");
	if (!IS_T(&(fmax), double (*)(double, double)))
		pc_fail("fmax() is not declared as double fmax(double, double)");
	if (IS_T(&(fmax), double (*)(double, double, int)))
		pc_fail("fmax() is declared without a prototype");
	if (!IS_T(&(fmaxf), float (*)(float, float)))
		pc_fail("fmaxf() is not declared as float fmaxf(float, float)");
	if (IS_T(&(fmaxf), float (*)(float, float, int)))
		pc_fail("fmaxf() is declared without a prototype");
	if (!IS_T(&(fmaxl), long double (*)(long double, long double)))
		pc_fail("fmaxl() is not declared as long double fmaxl(long double, long double)");
	if (IS_T(&(fmaxl), long double (*)(long double, long double, int)))
		pc_fail("fmaxl() is declared without a prototype");
	if (!IS_T(&(fmin), double (*)(double, double)))
		pc_fail("fmin() is not declared as double fmin(double, double)");
	if (IS_T(&(fmin), double (*)(double, double, int)))
		pc_fail("fmin() is declared without a prototype");
	if (!IS_T(&(fminf), float (*)(float, float)))
		pc_fail("fminf() is not declared as float fminf(float, float)");
	if (IS_T(&(fminf), float (*)(float, float, int)))
		pc_fail("fminf() is declared without a prototype");
	if (!IS_T(&(fminl), long double (*)(long double, long double)))
		pc_fail("fminl() is not declared as long double fminl(long double, long double)");
	if (IS_T(&(fminl), long double (*)(long double, long double, int)))
		pc_fail("fminl() is declared without a prototype");
	if (!IS_T(&(fmod), double (*)(double, double)))
		pc_fail("fmod() is not declared as double fmod(double, double)");
	if (IS_T(&(fmod), double (*)(double, double, int)))
		pc_fail("fmod() is declared without a prototype");
	if (!IS_T(&(fmodf), float (*)(float, float)))
		pc_fail("fmodf() is not declared as float fmodf(float, float)");
	if (IS_T(&(fmodf), float (*)(float, float, int)))
		pc_fail("fmodf() is declared without a prototype");
	if (!IS_T(&(fmodl), long double (*)(long double, long double)))
		pc_fail("fmodl() is not declared as long double fmodl(long double, long double)");
	if (IS_T(&(fmodl), long double (*)(long double, long double, int)))
		pc_fail("fmodl() is declared without a prototype");
	if (!IS_T(&(frexp), double (*)(double, int *)))
		pc_fail("frexp() is not declared as double frexp(double, int *)");
	if (IS_T(&(frexp), double (*)(double, int *, int)))
		pc_fail("frexp() is declared without a prototype");
	if (!IS_T(&(frexpf), float (*)(float, int *)))
		pc_fail("frexpf() is not declared as float frexpf(float, int *)");
	if (IS_T(&(frexpf), float (*)(float, int *, int)))
		pc_fail("frexpf() is declared without a prototype");
	if (!IS_T(&(frexpl), long double (*)(long double, int *)))
		pc_fail("frexpl() is not declared as long double frexpl(long double, int *)");
	if (IS_T(&(frexpl), long double (*)(long double, int *, int)))
		pc_fail("frexpl() is declared without a prototype");
	if (!IS_T(&(hypot), double (*)(double, double)))
		pc_fail("hypot() is not declared as double hypot(double, double)");
	if (IS_T(&(hypot), double (*)(double, double, int)))
		pc_fail("hypot() is declared without a prototype");
	if (!IS_T(&(hypotf), float (*)(float, float)))
		pc_fail("hypotf() is not declared as float hypotf(float, float)");
	if (IS_T(&(hypotf), float (*)(float, float, int)))
		pc_fail("hypotf() is declared without a prototype");
	if (!IS_T(&(hypotl), long double (*)(long double, long double)))
		pc_fail("hypotl() is not declared as long double hypotl(long double, long double)");
	if (IS_T(&(hypotl), long double (*)(long double, long double, int)))
		pc_fail("hypotl() is declared without a prototype");
	if (!IS_T(&(ilogb), int (*)(double)))
		pc_fail("ilogb() is not declared as int ilogb(double)");
	if (IS_T(&(ilogb), int (*)(double, int)))
		pc_fail("ilogb() is declared without a prototype");
	if (!IS_T(&(ilogbf), int (*)(float)))
		pc_fail("ilogbf() is not declared as int ilogbf(float)");
	if (IS_T(&(ilogbf), int (*)(float, int)))
		pc_fail("ilogbf() is declared without a prototype");
	if (!IS_T(&(ilogbl), int (*)(long double)))
		pc_fail("ilogbl() is not declared as int ilogbl(long double)");
	if (IS_T(&(ilogbl), int (*)(long double, int)))
		pc_fail("ilogbl() is declared without a prototype");
	if (!IS_T(&(ldexp), double (*)(double, int)))
		pc_fail("ldexp() is not declared as double ldexp(double, int)");
	if (IS_T(&(ldexp), double (*)(double, int, int)))
		pc_fail("ldexp() is declared without a prototype");
	if (!IS_T(&(ldexpf), float (*)(float, int)))
		pc_fail("ldexpf() is not declared as float ldexpf(float, int)");
	if (IS_T(&(ldexpf), float (*)(float, int, int)))
		pc_fail("ldexpf() is declared without a prototype");
	if (!IS_T(&(ldexpl), long double (*)(long double, int)))
		pc_fail("ldexpl() is not declared as long double ldexpl(long double, int)");
	if (IS_T(&(ldexpl), long double (*)(long double, int, int)))
		pc_fail("ldexpl() is declared without a prototype");
	if (!IS_T(&(lgamma), double (*)(double)))
		pc_fail("lgamma() is not declared as double lgamma(double)");
	if (IS_T(&(lgamma), double (*)(double, int)))
		pc_fail("lgamma() is declared without a prototype");
	if (!IS_T(&(lgammaf), float (*)(float)))
		pc_fail("lgammaf() is not declared as float lgammaf(float)");
	if (IS_T(&(lgammaf), float (*)(float, int)))
		pc_fail("lgammaf() is declared without a prototype");
	if (!IS_T(&(lgammal), long double (*)(long double)))
		pc_fail("lgammal() is not declared as long double lgammal(long double)");
	if (IS_T(&(lgammal), long double (*)(long double, int)))
		pc_fail("lgammal() is declared without a prototype");
	if (!IS_T(&(llrint), long long (*)(double)))
		pc_fail("llrint() is not declared as long long llrint(double)");
	if (IS_T(&(llrint), long long (*)(double, int)))
		pc_fail("llrint() is declared without a prototype");
	if (!IS_T(&(llrintf), long long (*)(float)))
		pc_fail("llrintf() is not declared as long long llrintf(float)");
	if (IS_T(&(llrintf), long long (*)(float, int)))
		pc_fail("llrintf() is declared without a prototype");
	if (!IS_T(&(llrintl), long long (*)(long double)))
		pc_fail("llrintl() is not declared as long long llrintl(long double)");
	if (IS_T(&(llrintl), long long (*)(long double, int)))
		pc_fail("llrintl() is declared without a prototype");
	if (!IS_T(&(llround), long long (*)(double)))
		pc_fail("llround() is not declared as long long llround(double)");
	if (IS_T(&(llround), long long (*)(double, int)))
		pc_fail("llround() is declared without a prototype");
	if (!IS_T(&(llroundf), long long (*)(float)))
		pc_fail("llroundf() is not declared as long long llroundf(float)");
	if (IS_T(&(llroundf), long long (*)(float, int)))
		pc_fail("llroundf() is declared without a prototype");
	if (!IS_T(&(llroundl), long long (*)(long double)))
		pc_fail("llroundl() is not declared as long long llroundl(long double)");
	if (IS_T(&(llroundl), long long (*)(long double, int)))
		pc_fail("llroundl() is declared without a prototype");
	if (!IS_T(&(log), double (*)(double)))
		pc_fail("log() is not declared as double log(double)");
	if (IS_T(&(log), double (*)(double, int)))
		pc_fail("log() is declared without a prototype");
	if (!IS_T(&(log10), double (*)(double)))
		pc_fail("log10() is not declared as double log10(double)");
	if (IS_T(&(log10), double (*)(double, int)))
		pc_fail("log10() is declared without a prototype");
	if (!IS_T(&(log10f), float (*)(float)))
		pc_fail("log10f() is not declared as float log10f(float)");
	if (IS_T(&(log10f), float (*)(float, int)))
		pc_fail("log10f() is declared without a prototype");
	if (!IS_T(&(log10l), long double (*)(long double)))
		pc_fail("log10l() is not declared as long double log10l(long double)");
	if (IS_T(&(log10l), long double (*)(long double, int)))
		pc_fail("log10l() is declared without a prototype");
	if (!IS_T(&(log1p), double (*)(double)))
		pc_fail("log1p() is not declared as double log1p(double)");
	if (IS_T(&(log1p), double (*)(double, int)))
		pc_fail("log1p() is declared without a prototype");
	if (!IS_T(&(log1pf), float (*)(float)))
		pc_fail("log1pf() is not declared as float log1pf(float)");
	if (IS_T(&(log1pf), float (*)(float, int)))
		pc_fail("log1pf() is declared without a prototype");
	if (!IS_T(&(log1pl), long double (*)(long double)))
		pc_fail("log1pl() is not declared as long double log1pl(long double)");
	if (IS_T(&(log1pl), long double (*)(long double, int)))
		pc_fail("log1pl() is declared without a prototype");
	if (!IS_T(&(log2), double (*)(double)))
		pc_fail("log2() is not declared as double log2(double)");
	if (IS_T(&(log2), double (*)(double, int)))
		pc_fail("log2() is declared without a prototype");
	if (!IS_T(&(log2f), float (*)(float)))
		pc_fail("log2f() is not declared as float log2f(float)");
	if (IS_T(&(log2f), float (*)(float, int)))
		pc_fail("log2f() is declared without a prototype");
	if (!IS_T(&(log2l), long double (*)(long double)))
		pc_fail("log2l() is not declared as long double log2l(long double)");
	if (IS_T(&(log2l), long double (*)(long double, int)))
		pc_fail("log2l() is declared without a prototype");
	if (!IS_T(&(logb), double (*)(double)))
		pc_fail("logb() is not declared as double logb(double)");
	if (IS_T(&(logb), double (*)(double, int)))
		pc_fail("logb() is declared without a prototype");
	if (!IS_T(&(logbf), float (*)(float)))
		pc_fail("logbf() is not declared as float logbf(float)");
	if (IS_T(&(logbf), float (*)(float, int)))
		pc_fail("logbf() is declared without a prototype");
	if (!IS_T(&(logbl), long double (*)(long double)))
		pc_fail("logbl() is not declared as long double logbl(long double)");
	if (IS_T(&(logbl), long double (*)(long double, int)))
		pc_fail("logbl() is declared without a prototype");
	if (!IS_T(&(logf), float (*)(float)))
		pc_fail("logf() is not declared as float logf(float)");
	if (IS_T(&(logf), float (*)(float, int)))
		pc_fail("logf() is declared without a prototype");
	if (!IS_T(&(logl), long double (*)(long double)))
		pc_fail("logl() is not declared as long double logl(long double)");
	if (IS_T(&(logl), long double (*)(long double, int)))
		pc_fail("logl() is declared without a prototype");
	if (!IS_T(&(lrint), long (*)(double)))
		pc_fail("lrint() is not declared as long lrint(double)");
	if (IS_T(&(lrint), long (*)(double, int)))
		pc_fail("lrint() is declared without a prototype");
	if (!IS_T(&(lrintf), long (*)(float)))
		pc_fail("lrintf() is not declared as long lrintf(float)");
	if (IS_T(&(lrintf), long (*)(float, int)))
		pc_fail("lrintf() is declared without a prototype");
	if (!IS_T(&(lrintl), long (*)(long double)))
		pc_fail("lrintl() is not declared as long lrintl(long double)");
	if (IS_T(&(lrintl), long (*)(long double, int)))
		pc_fail("lrintl() is declared without a prototype");
	if (!IS_T(&(lround), long (*)(double)))
		pc_fail("lround() is not declared as long lround(double)");
	if (IS_T(&(lround), long (*)(double, int)))
		pc_fail("lround() is declared without a prototype");
	if (!IS_T(&(lroundf), long (*)(float)))
		pc_fail("lroundf() is not declared as long lroundf(float)");
	if (IS_T(&(lroundf), long (*)(float, int)))
		pc_fail("lroundf() is declared without a prototype");
	if (!IS_T(&(lroundl), long (*)(long double)))
		pc_fail("lroundl() is not declared as long lroundl(long double)");
	if (IS_T(&(lroundl), long (*)(long double, int)))
		pc_fail("lroundl() is declared without a prototype");
	if (!IS_T(&(modf), double (*)(double, double *)))
		pc_fail("modf() is not declared as double modf(double, double *)");
	if (IS_T(&(modf), double (*)(double, double *, int)))
		pc_fail("modf() is declared without a prototype");
	if (!IS_T(&(modff), float (*)(float, float *)))
		pc_fail("modff() is not declared as float modff(float, float *)");
	if (IS_T(&(modff), float (*)(float, float *, int)))
		pc_fail("modff() is declared without a prototype");
	if (!IS_T(&(modfl), long double (*)(long double, long double *)))
		pc_fail("modfl() is not declared as long double modfl(long double, long double *)");
	if (IS_T(&(modfl), long double (*)(long double, long double *, int)))
		pc_fail("modfl() is declared without a prototype");
	if (!IS_T(&(nan), double (*)(const char *)))
		pc_fail("nan() is not declared as double nan(const char *)");
	if (IS_T(&(nan), double (*)(const char *, int)))
		pc_fail("nan() is declared without a prototype");
	if (!IS_T(&(nanf), float (*)(const char *)))
		pc_fail("nanf() is not declared as float nanf(const char *)");
	if (IS_T(&(nanf), float (*)(const char *, int)))
		pc_fail("nanf() is declared without a prototype");
	if (!IS_T(&(nanl), long double (*)(const char *)))
		pc_fail("nanl() is not declared as long double nanl(const char *)");
	if (IS_T(&(nanl), long double (*)(const char *, int)))
		pc_fail("nanl() is declared without a prototype");
	if (!IS_T(&(nearbyint), double (*)(double)))
		pc_fail("nearbyint() is not declared as double nearbyint(double)");
	if (IS_T(&(nearbyint), double (*)(double, int)))
		pc_fail("nearbyint() is declared without a prototype");
	if (!IS_T(&(nearbyintf), float (*)(float)))
		pc_fail("nearbyintf() is not declared as float nearbyintf(float)");
	if (IS_T(&(nearbyintf), float (*)(float, int)))
		pc_fail("nearbyintf() is declared without a prototype");
	if (!IS_T(&(nearbyintl), long double (*)(long double)))
		pc_fail("nearbyintl() is not declared as long double nearbyintl(long double)");
	if (IS_T(&(nearbyintl), long double (*)(long double, int)))
		pc_fail("nearbyintl() is declared without a prototype");
	if (!IS_T(&(nextafter), double (*)(double, double)))
		pc_fail("nextafter() is not declared as double nextafter(double, double)");
	if (IS_T(&(nextafter), double (*)(double, double, int)))
		pc_fail("nextafter() is declared without a prototype");
	if (!IS_T(&(nextafterf), float (*)(float, float)))
		pc_fail("nextafterf() is not declared as float nextafterf(float, float)");
	if (IS_T(&(nextafterf), float (*)(float, float, int)))
		pc_fail("nextafterf() is declared without a prototype");
	if (!IS_T(&(nextafterl), long double (*)(long double, long double)))
		pc_fail("nextafterl() is not declared as long double nextafterl(long double, long double)");
	if (IS_T(&(nextafterl), long double (*)(long double, long double, int)))
		pc_fail("nextafterl() is declared without a prototype");
	if (!IS_T(&(nexttoward), double (*)(double, long double)))
		pc_fail("nexttoward() is not declared as double nexttoward(double, long double)");
	if (IS_T(&(nexttoward), double (*)(double, long double, int)))
		pc_fail("nexttoward() is declared without a prototype");
	if (!IS_T(&(nexttowardf), float (*)(float, long double)))
		pc_fail("nexttowardf() is not declared as float nexttowardf(float, long double)");
	if (IS_T(&(nexttowardf), float (*)(float, long double, int)))
		pc_fail("nexttowardf() is declared without a prototype");
	if (!IS_T(&(nexttowardl), long double (*)(long double, long double)))
		pc_fail("nexttowardl() is not declared as long double nexttowardl(long double, long double)");
	if (IS_T(&(nexttowardl), long double (*)(long double, long double, int)))
		pc_fail("nexttowardl() is declared without a prototype");
	if (!IS_T(&(pow), double (*)(double, double)))
		pc_fail("pow() is not declared as double pow(double, double)");
	if (IS_T(&(pow), double (*)(double, double, int)))
		pc_fail("pow() is declared without a prototype");
	if (!IS_T(&(powf), float (*)(float, float)))
		pc_fail("powf() is not declared as float powf(float, float)");
	if (IS_T(&(powf), float (*)(float, float, int)))
		pc_fail("powf() is declared without a prototype");
	if (!IS_T(&(powl), long double (*)(long double, long double)))
		pc_fail("powl() is not declared as long double powl(long double, long double)");
	if (IS_T(&(powl), long double (*)(long double, long double, int)))
		pc_fail("powl() is declared without a prototype");
	if (!IS_T(&(remainder), double (*)(double, double)))
		pc_fail("remainder() is not declared as double remainder(double, double)");
	if (IS_T(&(remainder), double (*)(double, double, int)))
		pc_fail("remainder() is declared without a prototype");
	if (!IS_T(&(remainderf), float (*)(float, float)))
		pc_fail("remainderf() is not declared as float remainderf(float, float)");
	if (IS_T(&(remainderf), float (*)(float, float, int)))
		pc_fail("remainderf() is declared without a prototype");
	if (!IS_T(&(remainderl), long double (*)(long double, long double)))
		pc_fail("remainderl() is not declared as long double remainderl(long double, long double)");
	if (IS_T(&(remainderl), long double (*)(long double, long double, int)))
		pc_fail("remainderl() is declared without a prototype");
	if (!IS_T(&(remquo), double (*)(double, double, int *)))
		pc_fail("remquo() is not declared as double remquo(double, double, int *)");
	if (IS_T(&(remquo), double (*)(double, double, int *, int)))
		pc_fail("remquo() is declared without a prototype");
	if (!IS_T(&(remquof), float (*)(float, float, int *)))
		pc_fail("remquof() is not declared as float remquof(float, float, int *)");
	if (IS_T(&(remquof), float (*)(float, float, int *, int)))
		pc_fail("remquof() is declared without a prototype");
	if (!IS_T(&(remquol), long double (*)(long double, long double, int *)))
		pc_fail("remquol() is not declared as long double remquol(long double, long double, int *)");
	if (IS_T(&(remquol), long double (*)(long double, long double, int *, int)))
		pc_fail("remquol() is declared without a prototype");
	if (!IS_T(&(rint), double (*)(double)))
		pc_fail("rint() is not declared as double rint(double)");
	if (IS_T(&(rint), double (*)(double, int)))
		pc_fail("rint() is declared without a prototype");
	if (!IS_T(&(rintf), float (*)(float)))
		pc_fail("rintf() is not declared as float rintf(float)");
	if (IS_T(&(rintf), float (*)(float, int)))
		pc_fail("rintf() is declared without a prototype");
	if (!IS_T(&(rintl), long double (*)(long double)))
		pc_fail("rintl() is not declared as long double rintl(long double)");
	if (IS_T(&(rintl), long double (*)(long double, int)))
		pc_fail("rintl() is declared without a prototype");
	if (!IS_T(&(round), double (*)(double)))
		pc_fail("round() is not declared as double round(double)");
	if (IS_T(&(round), double (*)(double, int)))
		pc_fail("round() is declared without a prototype");
	if (!IS_T(&(roundf), float (*)(float)))
		pc_fail("roundf() is not declared as float roundf(float)");
	if (IS_T(&(roundf), float (*)(float, int)))
		pc_fail("roundf() is declared without a prototype");
	if (!IS_T(&(roundl), long double (*)(long double)))
		pc_fail("roundl() is not declared as long double roundl(long double)");
	if (IS_T(&(roundl), long double (*)(long double, int)))
		pc_fail("roundl() is declared without a prototype");
	if (!IS_T(&(scalbln), double (*)(double, long)))
		pc_fail("scalbln() is not declared as double scalbln(double, long)");
	if (IS_T(&(scalbln), double (*)(double, long, int)))
		pc_fail("scalbln() is declared without a prototype");
	if (!IS_T(&(scalblnf), float (*)(float, long)))
		pc_fail("scalblnf() is not declared as float scalblnf(float, long)");
	if (IS_T(&(scalblnf), float (*)(float, long, int)))
		pc_fail("scalblnf() is declared without a prototype");
	if (!IS_T(&(scalblnl), long double (*)(long double, long)))
		pc_fail("scalblnl() is not declared as long double scalblnl(long double, long)");
	if (IS_T(&(scalblnl), long double (*)(long double, long, int)))
		pc_fail("scalblnl() is declared without a prototype");
	if (!IS_T(&(scalbn), double (*)(double, int)))
		pc_fail("scalbn() is not declared as double scalbn(double, int)");
	if (IS_T(&(scalbn), double (*)(double, int, int)))
		pc_fail("scalbn() is declared without a prototype");
	if (!IS_T(&(scalbnf), float (*)(float, int)))
		pc_fail("scalbnf() is not declared as float scalbnf(float, int)");
	if (IS_T(&(scalbnf), float (*)(float, int, int)))
		pc_fail("scalbnf() is declared without a prototype");
	if (!IS_T(&(scalbnl), long double (*)(long double, int)))
		pc_fail("scalbnl() is not declared as long double scalbnl(long double, int)");
	if (IS_T(&(scalbnl), long double (*)(long double, int, int)))
		pc_fail("scalbnl() is declared without a prototype");
	if (!IS_T(&(sin), double (*)(double)))
		pc_fail("sin() is not declared as double sin(double)");
	if (IS_T(&(sin), double (*)(double, int)))
		pc_fail("sin() is declared without a prototype");
	if (!IS_T(&(sinf), float (*)(float)))
		pc_fail("sinf() is not declared as float sinf(float)");
	if (IS_T(&(sinf), float (*)(float, int)))
		pc_fail("sinf() is declared without a prototype");
	if (!IS_T(&(sinh), double (*)(double)))
		pc_fail("sinh() is not declared as double sinh(double)");
	if (IS_T(&(sinh), double (*)(double, int)))
		pc_fail("sinh() is declared without a prototype");
	if (!IS_T(&(sinhf), float (*)(float)))
		pc_fail("sinhf() is not declared as float sinhf(float)");
	if (IS_T(&(sinhf), float (*)(float, int)))
		pc_fail("sinhf() is declared without a prototype");
	if (!IS_T(&(sinhl), long double (*)(long double)))
		pc_fail("sinhl() is not declared as long double sinhl(long double)");
	if (IS_T(&(sinhl), long double (*)(long double, int)))
		pc_fail("sinhl() is declared without a prototype");
	if (!IS_T(&(sinl), long double (*)(long double)))
		pc_fail("sinl() is not declared as long double sinl(long double)");
	if (IS_T(&(sinl), long double (*)(long double, int)))
		pc_fail("sinl() is declared without a prototype");
	if (!IS_T(&(sqrt), double (*)(double)))
		pc_fail("sqrt() is not declared as double sqrt(double)");
	if (IS_T(&(sqrt), double (*)(double, int)))
		pc_fail("sqrt() is declared without a prototype");
	if (!IS_T(&(sqrtf), float (*)(float)))
		pc_fail("sqrtf() is not declared as float sqrtf(float)");
	if (IS_T(&(sqrtf), float (*)(float, int)))
		pc_fail("sqrtf() is declared without a prototype");
	if (!IS_T(&(sqrtl), long double (*)(long double)))
		pc_fail("sqrtl() is not declared as long double sqrtl(long double)");
	if (IS_T(&(sqrtl), long double (*)(long double, int)))
		pc_fail("sqrtl() is declared without a prototype");
	if (!IS_T(&(tan), double (*)(double)))
		pc_fail("tan() is not declared as double tan(double)");
	if (IS_T(&(tan), double (*)(double, int)))
		pc_fail("tan() is declared without a prototype");
	if (!IS_T(&(tanf), float (*)(float)))
		pc_fail("tanf() is not declared as float tanf(float)");
	if (IS_T(&(tanf), float (*)(float, int)))
		pc_fail("tanf() is declared without a prototype");
	if (!IS_T(&(tanh), double (*)(double)))
		pc_fail("tanh() is not declared as double tanh(double)");
	if (IS_T(&(tanh), double (*)(double, int)))
		pc_fail("tanh() is declared without a prototype");
	if (!IS_T(&(tanhf), float (*)(float)))
		pc_fail("tanhf() is not declared as float tanhf(float)");
	if (IS_T(&(tanhf), float (*)(float, int)))
		pc_fail("tanhf() is declared without a prototype");
	if (!IS_T(&(tanhl), long double (*)(long double)))
		pc_fail("tanhl() is not declared as long double tanhl(long double)");
	if (IS_T(&(tanhl), long double (*)(long double, int)))
		pc_fail("tanhl() is declared without a prototype");
	if (!IS_T(&(tanl), long double (*)(long double)))
		pc_fail("tanl() is not declared as long double tanl(long double)");
	if (IS_T(&(tanl), long double (*)(long double, int)))
		pc_fail("tanl() is declared without a prototype");
	if (!IS_T(&(tgamma), double (*)(double)))
		pc_fail("tgamma() is not declared as double tgamma(double)");
	if (IS_T(&(tgamma), double (*)(double, int)))
		pc_fail("tgamma() is declared without a prototype");
	if (!IS_T(&(tgammaf), float (*)(float)))
		pc_fail("tgammaf() is not declared as float tgammaf(float)");
	if (IS_T(&(tgammaf), float (*)(float, int)))
		pc_fail("tgammaf() is declared without a prototype");
	if (!IS_T(&(tgammal), long double (*)(long double)))
		pc_fail("tgammal() is not declared as long double tgammal(long double)");
	if (IS_T(&(tgammal), long double (*)(long double, int)))
		pc_fail("tgammal() is declared without a prototype");
	if (!IS_T(&(trunc), double (*)(double)))
		pc_fail("trunc() is not declared as double trunc(double)");
	if (IS_T(&(trunc), double (*)(double, int)))
		pc_fail("trunc() is declared without a prototype");
	if (!IS_T(&(truncf), float (*)(float)))
		pc_fail("truncf() is not declared as float truncf(float)");
	if (IS_T(&(truncf), float (*)(float, int)))
		pc_fail("truncf() is declared without a prototype");
	if (!IS_T(&(truncl), long double (*)(long double)))
		pc_fail("truncl() is not declared as long double truncl(long double)");
	if (IS_T(&(truncl), long double (*)(long double, int)))
		pc_fail("truncl() is declared without a prototype");
	pc_pass();
}
