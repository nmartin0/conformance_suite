#include <stdio.h>
#include <math.h>
#include <fenv.h>
static void show(const char *n, double v) { int e = fetestexcept(FE_ALL_EXCEPT); printf("%s -> %g flags: div=%d inv=%d ovf=%d unf=%d inx=%d\n", n, v, !!(e&FE_DIVBYZERO), !!(e&FE_INVALID), !!(e&FE_OVERFLOW), !!(e&FE_UNDERFLOW), !!(e&FE_INEXACT)); feclearexcept(FE_ALL_EXCEPT); }
int main(void)
{
	volatile double z = 0.0, big = 1000.0, one = 1.0, m1 = -1.0, mx = 1.7e308;
	volatile double r;
	feclearexcept(FE_ALL_EXCEPT);
	r = one / z; show("1.0/0.0 (hardware)", r);
	r = mx * 10.0; show("max*10 (hardware)", r);
	r = sqrt(m1); show("sqrt(-1)", r);
	r = log(z); show("log(0)", r);
	r = exp(big); show("exp(1000)", r);
	r = cosh(big); show("cosh(1000)", r);
	r = log(m1); show("log(-1)", r);
	{ volatile long double lz = 0.0L, lm = -1.0L; volatile long double lr = logl(lz); int e = fetestexcept(FE_ALL_EXCEPT); printf("logl(0) flags=0x%x\n", e); feclearexcept(FE_ALL_EXCEPT); lr = sqrtl(lm); e = fetestexcept(FE_ALL_EXCEPT); printf("sqrtl(-1) flags=0x%x\n", e); (void)lr; }
	return 0;
}
