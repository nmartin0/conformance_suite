/*
 * The comparison macros agree with the operators for mixed operand types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isgreater.009
 */
#include "mt.h"

int main(void)
{
	pc_begin("XSH.isgreater.009");
	pc_test();
	volatile float f = 0.1f;
	volatile double d = 0.1;
	volatile long double l = 0.1L;

	pc_check((isgreater(f, d) != 0) == (f > d) && (isgreater(d, f) != 0) == (d > f), "isgreater() on float and double disagrees with >");
	pc_check((isgreaterequal(f, d) != 0) == (f >= d) && (isgreaterequal(d, f) != 0) == (d >= f), "isgreaterequal() on float and double disagrees with >=");
	pc_check((isless(f, d) != 0) == (f < d) && (isless(d, f) != 0) == (d < f), "isless() on float and double disagrees with <");
	pc_check((islessequal(f, d) != 0) == (f <= d) && (islessequal(d, f) != 0) == (d <= f), "islessequal() on float and double disagrees with <=");
	pc_check((islessgreater(f, d) != 0) == (f < d || f > d) && (islessgreater(d, f) != 0) == (d < f || d > f), "islessgreater() on float and double disagrees with < or >");
	pc_check((isgreater(d, l) != 0) == (d > l) && (isgreater(l, d) != 0) == (l > d), "isgreater() on double and long double disagrees with >");
	pc_check((isgreaterequal(d, l) != 0) == (d >= l) && (isgreaterequal(l, d) != 0) == (l >= d), "isgreaterequal() on double and long double disagrees with >=");
	pc_check((isless(d, l) != 0) == (d < l) && (isless(l, d) != 0) == (l < d), "isless() on double and long double disagrees with <");
	pc_check((islessequal(d, l) != 0) == (d <= l) && (islessequal(l, d) != 0) == (l <= d), "islessequal() on double and long double disagrees with <=");
	pc_check((islessgreater(d, l) != 0) == (d < l || d > l) && (islessgreater(l, d) != 0) == (l < d || l > d), "islessgreater() on double and long double disagrees with < or >");
	pc_check((isgreater(f, l) != 0) == (f > l) && (isless(f, l) != 0) == (f < l) && (islessgreater(l, f) != 0) == (l < f || l > f), "the macros on float and long double disagree with the operators");
	pc_pass();
}
