/*
 * atof: hex inf nan negative zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atof.003
 */
#include "pc.h"
#include <stdlib.h>
#include <math.h>
static double (*volatile f_atof)(const char *) = atof;

int main(void)
{
	double r;
	pc_begin("XSH.atof.003");
	pc_test();
	pc_check(f_atof("0x1p4") == 16.0, "atof(\"0x1p4\") did not give 16");
	pc_check(f_atof("0x1.8p1") == 3.0, "atof(\"0x1.8p1\") did not give 3");
	pc_check(f_atof("0x10") == 16.0, "atof(\"0x10\") did not give 16");
	pc_check(f_atof("inf") == HUGE_VAL, "atof(\"inf\") did not give HUGE_VAL");
	pc_check(f_atof("INFINITY") == HUGE_VAL, "atof(\"INFINITY\") did not give HUGE_VAL");
	pc_check(f_atof("-inf") == -HUGE_VAL, "atof(\"-inf\") did not give -HUGE_VAL");
	r = f_atof("-0");
	pc_check(r == 0.0 && signbit(r), "atof(\"-0\") did not give a negative zero");
	r = f_atof("nan");
	pc_check(r != r || r == 0.0, "atof(\"nan\") gave neither a NaN nor 0");
	pc_pass();
}
