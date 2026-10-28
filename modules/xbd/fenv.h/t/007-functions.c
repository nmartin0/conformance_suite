/*
 * <fenv.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fenv.h.007
 *
 * Only <fenv.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <fenv.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.fenv.h.007");
	pc_test();
	if (!IS_T(&(feclearexcept), int (*)(int)))
		pc_fail("feclearexcept() is not declared as int feclearexcept(int)");
	if (IS_T(&(feclearexcept), int (*)(int, int)))
		pc_fail("feclearexcept() is declared without a prototype");
	if (!IS_T(&(fegetenv), int (*)(fenv_t *)))
		pc_fail("fegetenv() is not declared as int fegetenv(fenv_t *)");
	if (IS_T(&(fegetenv), int (*)(fenv_t *, int)))
		pc_fail("fegetenv() is declared without a prototype");
	if (!IS_T(&(fegetexceptflag), int (*)(fexcept_t *, int)))
		pc_fail("fegetexceptflag() is not declared as int fegetexceptflag(fexcept_t *, int)");
	if (IS_T(&(fegetexceptflag), int (*)(fexcept_t *, int, int)))
		pc_fail("fegetexceptflag() is declared without a prototype");
	if (!IS_T(&(fegetround), int (*)(void)))
		pc_fail("fegetround() is not declared as int fegetround(void)");
	if (IS_T(&(fegetround), int (*)(int)))
		pc_fail("fegetround() is declared without a prototype");
	if (!IS_T(&(feholdexcept), int (*)(fenv_t *)))
		pc_fail("feholdexcept() is not declared as int feholdexcept(fenv_t *)");
	if (IS_T(&(feholdexcept), int (*)(fenv_t *, int)))
		pc_fail("feholdexcept() is declared without a prototype");
	if (!IS_T(&(feraiseexcept), int (*)(int)))
		pc_fail("feraiseexcept() is not declared as int feraiseexcept(int)");
	if (IS_T(&(feraiseexcept), int (*)(int, int)))
		pc_fail("feraiseexcept() is declared without a prototype");
	if (!IS_T(&(fesetenv), int (*)(const fenv_t *)))
		pc_fail("fesetenv() is not declared as int fesetenv(const fenv_t *)");
	if (IS_T(&(fesetenv), int (*)(const fenv_t *, int)))
		pc_fail("fesetenv() is declared without a prototype");
	if (!IS_T(&(fesetexceptflag), int (*)(const fexcept_t *, int)))
		pc_fail("fesetexceptflag() is not declared as int fesetexceptflag(const fexcept_t *, int)");
	if (IS_T(&(fesetexceptflag), int (*)(const fexcept_t *, int, int)))
		pc_fail("fesetexceptflag() is declared without a prototype");
	if (!IS_T(&(fesetround), int (*)(int)))
		pc_fail("fesetround() is not declared as int fesetround(int)");
	if (IS_T(&(fesetround), int (*)(int, int)))
		pc_fail("fesetround() is declared without a prototype");
	if (!IS_T(&(fetestexcept), int (*)(int)))
		pc_fail("fetestexcept() is not declared as int fetestexcept(int)");
	if (IS_T(&(fetestexcept), int (*)(int, int)))
		pc_fail("fetestexcept() is declared without a prototype");
	if (!IS_T(&(feupdateenv), int (*)(const fenv_t *)))
		pc_fail("feupdateenv() is not declared as int feupdateenv(const fenv_t *)");
	if (IS_T(&(feupdateenv), int (*)(const fenv_t *, int)))
		pc_fail("feupdateenv() is declared without a prototype");
	pc_pass();
}
