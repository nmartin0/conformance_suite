/*
 * <math.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.017
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
	pc_begin("XBD.math.h.017");
	pc_test();
	if (!IS_T(&(j0), double (*)(double)))
		pc_fail("j0() is not declared as double j0(double)");
	if (IS_T(&(j0), double (*)(double, int)))
		pc_fail("j0() is declared without a prototype");
	if (!IS_T(&(j1), double (*)(double)))
		pc_fail("j1() is not declared as double j1(double)");
	if (IS_T(&(j1), double (*)(double, int)))
		pc_fail("j1() is declared without a prototype");
	if (!IS_T(&(jn), double (*)(int, double)))
		pc_fail("jn() is not declared as double jn(int, double)");
	if (IS_T(&(jn), double (*)(int, double, int)))
		pc_fail("jn() is declared without a prototype");
	if (!IS_T(&(y0), double (*)(double)))
		pc_fail("y0() is not declared as double y0(double)");
	if (IS_T(&(y0), double (*)(double, int)))
		pc_fail("y0() is declared without a prototype");
	if (!IS_T(&(y1), double (*)(double)))
		pc_fail("y1() is not declared as double y1(double)");
	if (IS_T(&(y1), double (*)(double, int)))
		pc_fail("y1() is declared without a prototype");
	if (!IS_T(&(yn), double (*)(int, double)))
		pc_fail("yn() is not declared as double yn(int, double)");
	if (IS_T(&(yn), double (*)(int, double, int)))
		pc_fail("yn() is declared without a prototype");
	pc_pass();
}
