/*
 * atol: wide values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atol.002
 */
#include "pc.h"
#include <stdlib.h>
#include <limits.h>
static long (*volatile f_atol)(const char *) = atol;
static long long (*volatile f_atoll)(const char *) = atoll;

int main(void)
{
	pc_begin("XSH.atol.002");
	pc_test();
	pc_check(f_atoll("3000000000") == 3000000000LL, "atoll(\"3000000000\") was wrong");
	pc_check(f_atoll("-3000000000") == -3000000000LL, "atoll(\"-3000000000\") was wrong");
	pc_check(f_atoll("5000000000000") == 5000000000000LL, "atoll(\"5000000000000\") was wrong");
	if (LONG_MAX > 2147483647L) {
		pc_check(f_atol("3000000000") == 3000000000L, "atol(\"3000000000\") was wrong");
		pc_check(f_atol("-3000000000") == -3000000000L, "atol(\"-3000000000\") was wrong");
	}
	pc_pass();
}
