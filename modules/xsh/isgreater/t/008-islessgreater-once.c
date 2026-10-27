/*
 * islessgreater() evaluates each argument once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isgreater.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double a[2];
	volatile int r;
	int k = 0, j = 0;

	a[0] = 1;
	a[1] = 2;
	r = islessgreater(a[k++], (double)5);
	pc_check(k == 1 && r != 0, "islessgreater(a[k++], 5) of type double evaluated its first argument %d times (result %d)", k, r);
	r = islessgreater((double)1, a[j++]);
	pc_check(j == 1 && r == 0, "islessgreater(1, a[j++]) of type double evaluated its second argument %d times (result %d)", j, r);
}

static void run_f(void)
{
	volatile float a[2];
	volatile int r;
	int k = 0, j = 0;

	a[0] = 1;
	a[1] = 2;
	r = islessgreater(a[k++], (float)5);
	pc_check(k == 1 && r != 0, "islessgreater(a[k++], 5) of type float evaluated its first argument %d times (result %d)", k, r);
	r = islessgreater((float)1, a[j++]);
	pc_check(j == 1 && r == 0, "islessgreater(1, a[j++]) of type float evaluated its second argument %d times (result %d)", j, r);
}

static void run_l(void)
{
	volatile long double a[2];
	volatile int r;
	int k = 0, j = 0;

	a[0] = 1;
	a[1] = 2;
	r = islessgreater(a[k++], (long double)5);
	pc_check(k == 1 && r != 0, "islessgreater(a[k++], 5) of type long double evaluated its first argument %d times (result %d)", k, r);
	r = islessgreater((long double)1, a[j++]);
	pc_check(j == 1 && r == 0, "islessgreater(1, a[j++]) of type long double evaluated its second argument %d times (result %d)", j, r);
}

int main(void)
{
	pc_begin("XSH.isgreater.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
