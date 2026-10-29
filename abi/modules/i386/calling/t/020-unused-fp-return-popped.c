/*
 * Code that ignores a floating-point return value still removes it from the x87 register stack.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.020
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.020")
#else
__attribute__((noinline)) float f_float(void) { return 1.5f; }
__attribute__((noinline)) double f_double(void) { return 2.5; }
__attribute__((noinline)) long double f_ldouble(void) { return 3.5L; }
static float (*volatile pf)(void) = f_float;
static double (*volatile pd)(void) = f_double;
static long double (*volatile pl)(void) = f_ldouble;
int main(void)
{
	pc_begin("ABI.i386.calling.020");
	pc_test();
	pf();
	pc_check(abi_x87_used() == 0, "%u x87 register(s) in use after ignoring a float return", abi_x87_used());
	pd();
	pc_check(abi_x87_used() == 0, "%u x87 register(s) in use after ignoring a double return", abi_x87_used());
	pl();
	pc_check(abi_x87_used() == 0, "%u x87 register(s) in use after ignoring a long double return", abi_x87_used());
	pc_pass();
}
#endif
