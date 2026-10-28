/*
 * <complex.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.008
 *
 * Only <complex.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <complex.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.complex.h.008");
	pc_test();
	if (!IS_T(&(cabs), double (*)(double complex)))
		pc_fail("cabs() is not declared as double cabs(double complex)");
	if (IS_T(&(cabs), double (*)(double complex, int)))
		pc_fail("cabs() is declared without a prototype");
	if (!IS_T(&(cabsf), float (*)(float complex)))
		pc_fail("cabsf() is not declared as float cabsf(float complex)");
	if (IS_T(&(cabsf), float (*)(float complex, int)))
		pc_fail("cabsf() is declared without a prototype");
	if (!IS_T(&(cabsl), long double (*)(long double complex)))
		pc_fail("cabsl() is not declared as long double cabsl(long double complex)");
	if (IS_T(&(cabsl), long double (*)(long double complex, int)))
		pc_fail("cabsl() is declared without a prototype");
	if (!IS_T(&(cacos), double complex (*)(double complex)))
		pc_fail("cacos() is not declared as double complex cacos(double complex)");
	if (IS_T(&(cacos), double complex (*)(double complex, int)))
		pc_fail("cacos() is declared without a prototype");
	if (!IS_T(&(cacosf), float complex (*)(float complex)))
		pc_fail("cacosf() is not declared as float complex cacosf(float complex)");
	if (IS_T(&(cacosf), float complex (*)(float complex, int)))
		pc_fail("cacosf() is declared without a prototype");
	if (!IS_T(&(cacosh), double complex (*)(double complex)))
		pc_fail("cacosh() is not declared as double complex cacosh(double complex)");
	if (IS_T(&(cacosh), double complex (*)(double complex, int)))
		pc_fail("cacosh() is declared without a prototype");
	if (!IS_T(&(cacoshf), float complex (*)(float complex)))
		pc_fail("cacoshf() is not declared as float complex cacoshf(float complex)");
	if (IS_T(&(cacoshf), float complex (*)(float complex, int)))
		pc_fail("cacoshf() is declared without a prototype");
	if (!IS_T(&(cacoshl), long double complex (*)(long double complex)))
		pc_fail("cacoshl() is not declared as long double complex cacoshl(long double complex)");
	if (IS_T(&(cacoshl), long double complex (*)(long double complex, int)))
		pc_fail("cacoshl() is declared without a prototype");
	if (!IS_T(&(cacosl), long double complex (*)(long double complex)))
		pc_fail("cacosl() is not declared as long double complex cacosl(long double complex)");
	if (IS_T(&(cacosl), long double complex (*)(long double complex, int)))
		pc_fail("cacosl() is declared without a prototype");
	if (!IS_T(&(carg), double (*)(double complex)))
		pc_fail("carg() is not declared as double carg(double complex)");
	if (IS_T(&(carg), double (*)(double complex, int)))
		pc_fail("carg() is declared without a prototype");
	if (!IS_T(&(cargf), float (*)(float complex)))
		pc_fail("cargf() is not declared as float cargf(float complex)");
	if (IS_T(&(cargf), float (*)(float complex, int)))
		pc_fail("cargf() is declared without a prototype");
	if (!IS_T(&(cargl), long double (*)(long double complex)))
		pc_fail("cargl() is not declared as long double cargl(long double complex)");
	if (IS_T(&(cargl), long double (*)(long double complex, int)))
		pc_fail("cargl() is declared without a prototype");
	if (!IS_T(&(casin), double complex (*)(double complex)))
		pc_fail("casin() is not declared as double complex casin(double complex)");
	if (IS_T(&(casin), double complex (*)(double complex, int)))
		pc_fail("casin() is declared without a prototype");
	if (!IS_T(&(casinf), float complex (*)(float complex)))
		pc_fail("casinf() is not declared as float complex casinf(float complex)");
	if (IS_T(&(casinf), float complex (*)(float complex, int)))
		pc_fail("casinf() is declared without a prototype");
	if (!IS_T(&(casinh), double complex (*)(double complex)))
		pc_fail("casinh() is not declared as double complex casinh(double complex)");
	if (IS_T(&(casinh), double complex (*)(double complex, int)))
		pc_fail("casinh() is declared without a prototype");
	if (!IS_T(&(casinhf), float complex (*)(float complex)))
		pc_fail("casinhf() is not declared as float complex casinhf(float complex)");
	if (IS_T(&(casinhf), float complex (*)(float complex, int)))
		pc_fail("casinhf() is declared without a prototype");
	if (!IS_T(&(casinhl), long double complex (*)(long double complex)))
		pc_fail("casinhl() is not declared as long double complex casinhl(long double complex)");
	if (IS_T(&(casinhl), long double complex (*)(long double complex, int)))
		pc_fail("casinhl() is declared without a prototype");
	if (!IS_T(&(casinl), long double complex (*)(long double complex)))
		pc_fail("casinl() is not declared as long double complex casinl(long double complex)");
	if (IS_T(&(casinl), long double complex (*)(long double complex, int)))
		pc_fail("casinl() is declared without a prototype");
	if (!IS_T(&(catan), double complex (*)(double complex)))
		pc_fail("catan() is not declared as double complex catan(double complex)");
	if (IS_T(&(catan), double complex (*)(double complex, int)))
		pc_fail("catan() is declared without a prototype");
	if (!IS_T(&(catanf), float complex (*)(float complex)))
		pc_fail("catanf() is not declared as float complex catanf(float complex)");
	if (IS_T(&(catanf), float complex (*)(float complex, int)))
		pc_fail("catanf() is declared without a prototype");
	if (!IS_T(&(catanh), double complex (*)(double complex)))
		pc_fail("catanh() is not declared as double complex catanh(double complex)");
	if (IS_T(&(catanh), double complex (*)(double complex, int)))
		pc_fail("catanh() is declared without a prototype");
	if (!IS_T(&(catanhf), float complex (*)(float complex)))
		pc_fail("catanhf() is not declared as float complex catanhf(float complex)");
	if (IS_T(&(catanhf), float complex (*)(float complex, int)))
		pc_fail("catanhf() is declared without a prototype");
	if (!IS_T(&(catanhl), long double complex (*)(long double complex)))
		pc_fail("catanhl() is not declared as long double complex catanhl(long double complex)");
	if (IS_T(&(catanhl), long double complex (*)(long double complex, int)))
		pc_fail("catanhl() is declared without a prototype");
	if (!IS_T(&(catanl), long double complex (*)(long double complex)))
		pc_fail("catanl() is not declared as long double complex catanl(long double complex)");
	if (IS_T(&(catanl), long double complex (*)(long double complex, int)))
		pc_fail("catanl() is declared without a prototype");
	if (!IS_T(&(ccos), double complex (*)(double complex)))
		pc_fail("ccos() is not declared as double complex ccos(double complex)");
	if (IS_T(&(ccos), double complex (*)(double complex, int)))
		pc_fail("ccos() is declared without a prototype");
	if (!IS_T(&(ccosf), float complex (*)(float complex)))
		pc_fail("ccosf() is not declared as float complex ccosf(float complex)");
	if (IS_T(&(ccosf), float complex (*)(float complex, int)))
		pc_fail("ccosf() is declared without a prototype");
	if (!IS_T(&(ccosh), double complex (*)(double complex)))
		pc_fail("ccosh() is not declared as double complex ccosh(double complex)");
	if (IS_T(&(ccosh), double complex (*)(double complex, int)))
		pc_fail("ccosh() is declared without a prototype");
	if (!IS_T(&(ccoshf), float complex (*)(float complex)))
		pc_fail("ccoshf() is not declared as float complex ccoshf(float complex)");
	if (IS_T(&(ccoshf), float complex (*)(float complex, int)))
		pc_fail("ccoshf() is declared without a prototype");
	if (!IS_T(&(ccoshl), long double complex (*)(long double complex)))
		pc_fail("ccoshl() is not declared as long double complex ccoshl(long double complex)");
	if (IS_T(&(ccoshl), long double complex (*)(long double complex, int)))
		pc_fail("ccoshl() is declared without a prototype");
	if (!IS_T(&(ccosl), long double complex (*)(long double complex)))
		pc_fail("ccosl() is not declared as long double complex ccosl(long double complex)");
	if (IS_T(&(ccosl), long double complex (*)(long double complex, int)))
		pc_fail("ccosl() is declared without a prototype");
	if (!IS_T(&(cexp), double complex (*)(double complex)))
		pc_fail("cexp() is not declared as double complex cexp(double complex)");
	if (IS_T(&(cexp), double complex (*)(double complex, int)))
		pc_fail("cexp() is declared without a prototype");
	if (!IS_T(&(cexpf), float complex (*)(float complex)))
		pc_fail("cexpf() is not declared as float complex cexpf(float complex)");
	if (IS_T(&(cexpf), float complex (*)(float complex, int)))
		pc_fail("cexpf() is declared without a prototype");
	if (!IS_T(&(cexpl), long double complex (*)(long double complex)))
		pc_fail("cexpl() is not declared as long double complex cexpl(long double complex)");
	if (IS_T(&(cexpl), long double complex (*)(long double complex, int)))
		pc_fail("cexpl() is declared without a prototype");
	if (!IS_T(&(cimag), double (*)(double complex)))
		pc_fail("cimag() is not declared as double cimag(double complex)");
	if (IS_T(&(cimag), double (*)(double complex, int)))
		pc_fail("cimag() is declared without a prototype");
	if (!IS_T(&(cimagf), float (*)(float complex)))
		pc_fail("cimagf() is not declared as float cimagf(float complex)");
	if (IS_T(&(cimagf), float (*)(float complex, int)))
		pc_fail("cimagf() is declared without a prototype");
	if (!IS_T(&(cimagl), long double (*)(long double complex)))
		pc_fail("cimagl() is not declared as long double cimagl(long double complex)");
	if (IS_T(&(cimagl), long double (*)(long double complex, int)))
		pc_fail("cimagl() is declared without a prototype");
	if (!IS_T(&(clog), double complex (*)(double complex)))
		pc_fail("clog() is not declared as double complex clog(double complex)");
	if (IS_T(&(clog), double complex (*)(double complex, int)))
		pc_fail("clog() is declared without a prototype");
	if (!IS_T(&(clogf), float complex (*)(float complex)))
		pc_fail("clogf() is not declared as float complex clogf(float complex)");
	if (IS_T(&(clogf), float complex (*)(float complex, int)))
		pc_fail("clogf() is declared without a prototype");
	if (!IS_T(&(clogl), long double complex (*)(long double complex)))
		pc_fail("clogl() is not declared as long double complex clogl(long double complex)");
	if (IS_T(&(clogl), long double complex (*)(long double complex, int)))
		pc_fail("clogl() is declared without a prototype");
	if (!IS_T(&(conj), double complex (*)(double complex)))
		pc_fail("conj() is not declared as double complex conj(double complex)");
	if (IS_T(&(conj), double complex (*)(double complex, int)))
		pc_fail("conj() is declared without a prototype");
	if (!IS_T(&(conjf), float complex (*)(float complex)))
		pc_fail("conjf() is not declared as float complex conjf(float complex)");
	if (IS_T(&(conjf), float complex (*)(float complex, int)))
		pc_fail("conjf() is declared without a prototype");
	if (!IS_T(&(conjl), long double complex (*)(long double complex)))
		pc_fail("conjl() is not declared as long double complex conjl(long double complex)");
	if (IS_T(&(conjl), long double complex (*)(long double complex, int)))
		pc_fail("conjl() is declared without a prototype");
	if (!IS_T(&(cpow), double complex (*)(double complex, double complex)))
		pc_fail("cpow() is not declared as double complex cpow(double complex, double complex)");
	if (IS_T(&(cpow), double complex (*)(double complex, double complex, int)))
		pc_fail("cpow() is declared without a prototype");
	if (!IS_T(&(cpowf), float complex (*)(float complex, float complex)))
		pc_fail("cpowf() is not declared as float complex cpowf(float complex, float complex)");
	if (IS_T(&(cpowf), float complex (*)(float complex, float complex, int)))
		pc_fail("cpowf() is declared without a prototype");
	if (!IS_T(&(cpowl), long double complex (*)(long double complex, long double complex)))
		pc_fail("cpowl() is not declared as long double complex cpowl(long double complex, long double complex)");
	if (IS_T(&(cpowl), long double complex (*)(long double complex, long double complex, int)))
		pc_fail("cpowl() is declared without a prototype");
	if (!IS_T(&(cproj), double complex (*)(double complex)))
		pc_fail("cproj() is not declared as double complex cproj(double complex)");
	if (IS_T(&(cproj), double complex (*)(double complex, int)))
		pc_fail("cproj() is declared without a prototype");
	if (!IS_T(&(cprojf), float complex (*)(float complex)))
		pc_fail("cprojf() is not declared as float complex cprojf(float complex)");
	if (IS_T(&(cprojf), float complex (*)(float complex, int)))
		pc_fail("cprojf() is declared without a prototype");
	if (!IS_T(&(cprojl), long double complex (*)(long double complex)))
		pc_fail("cprojl() is not declared as long double complex cprojl(long double complex)");
	if (IS_T(&(cprojl), long double complex (*)(long double complex, int)))
		pc_fail("cprojl() is declared without a prototype");
	if (!IS_T(&(creal), double (*)(double complex)))
		pc_fail("creal() is not declared as double creal(double complex)");
	if (IS_T(&(creal), double (*)(double complex, int)))
		pc_fail("creal() is declared without a prototype");
	if (!IS_T(&(crealf), float (*)(float complex)))
		pc_fail("crealf() is not declared as float crealf(float complex)");
	if (IS_T(&(crealf), float (*)(float complex, int)))
		pc_fail("crealf() is declared without a prototype");
	if (!IS_T(&(creall), long double (*)(long double complex)))
		pc_fail("creall() is not declared as long double creall(long double complex)");
	if (IS_T(&(creall), long double (*)(long double complex, int)))
		pc_fail("creall() is declared without a prototype");
	if (!IS_T(&(csin), double complex (*)(double complex)))
		pc_fail("csin() is not declared as double complex csin(double complex)");
	if (IS_T(&(csin), double complex (*)(double complex, int)))
		pc_fail("csin() is declared without a prototype");
	if (!IS_T(&(csinf), float complex (*)(float complex)))
		pc_fail("csinf() is not declared as float complex csinf(float complex)");
	if (IS_T(&(csinf), float complex (*)(float complex, int)))
		pc_fail("csinf() is declared without a prototype");
	if (!IS_T(&(csinh), double complex (*)(double complex)))
		pc_fail("csinh() is not declared as double complex csinh(double complex)");
	if (IS_T(&(csinh), double complex (*)(double complex, int)))
		pc_fail("csinh() is declared without a prototype");
	if (!IS_T(&(csinhf), float complex (*)(float complex)))
		pc_fail("csinhf() is not declared as float complex csinhf(float complex)");
	if (IS_T(&(csinhf), float complex (*)(float complex, int)))
		pc_fail("csinhf() is declared without a prototype");
	if (!IS_T(&(csinhl), long double complex (*)(long double complex)))
		pc_fail("csinhl() is not declared as long double complex csinhl(long double complex)");
	if (IS_T(&(csinhl), long double complex (*)(long double complex, int)))
		pc_fail("csinhl() is declared without a prototype");
	if (!IS_T(&(csinl), long double complex (*)(long double complex)))
		pc_fail("csinl() is not declared as long double complex csinl(long double complex)");
	if (IS_T(&(csinl), long double complex (*)(long double complex, int)))
		pc_fail("csinl() is declared without a prototype");
	if (!IS_T(&(csqrt), double complex (*)(double complex)))
		pc_fail("csqrt() is not declared as double complex csqrt(double complex)");
	if (IS_T(&(csqrt), double complex (*)(double complex, int)))
		pc_fail("csqrt() is declared without a prototype");
	if (!IS_T(&(csqrtf), float complex (*)(float complex)))
		pc_fail("csqrtf() is not declared as float complex csqrtf(float complex)");
	if (IS_T(&(csqrtf), float complex (*)(float complex, int)))
		pc_fail("csqrtf() is declared without a prototype");
	if (!IS_T(&(csqrtl), long double complex (*)(long double complex)))
		pc_fail("csqrtl() is not declared as long double complex csqrtl(long double complex)");
	if (IS_T(&(csqrtl), long double complex (*)(long double complex, int)))
		pc_fail("csqrtl() is declared without a prototype");
	if (!IS_T(&(ctan), double complex (*)(double complex)))
		pc_fail("ctan() is not declared as double complex ctan(double complex)");
	if (IS_T(&(ctan), double complex (*)(double complex, int)))
		pc_fail("ctan() is declared without a prototype");
	if (!IS_T(&(ctanf), float complex (*)(float complex)))
		pc_fail("ctanf() is not declared as float complex ctanf(float complex)");
	if (IS_T(&(ctanf), float complex (*)(float complex, int)))
		pc_fail("ctanf() is declared without a prototype");
	if (!IS_T(&(ctanh), double complex (*)(double complex)))
		pc_fail("ctanh() is not declared as double complex ctanh(double complex)");
	if (IS_T(&(ctanh), double complex (*)(double complex, int)))
		pc_fail("ctanh() is declared without a prototype");
	if (!IS_T(&(ctanhf), float complex (*)(float complex)))
		pc_fail("ctanhf() is not declared as float complex ctanhf(float complex)");
	if (IS_T(&(ctanhf), float complex (*)(float complex, int)))
		pc_fail("ctanhf() is declared without a prototype");
	if (!IS_T(&(ctanhl), long double complex (*)(long double complex)))
		pc_fail("ctanhl() is not declared as long double complex ctanhl(long double complex)");
	if (IS_T(&(ctanhl), long double complex (*)(long double complex, int)))
		pc_fail("ctanhl() is declared without a prototype");
	if (!IS_T(&(ctanl), long double complex (*)(long double complex)))
		pc_fail("ctanl() is not declared as long double complex ctanl(long double complex)");
	if (IS_T(&(ctanl), long double complex (*)(long double complex, int)))
		pc_fail("ctanl() is declared without a prototype");
	pc_pass();
}
