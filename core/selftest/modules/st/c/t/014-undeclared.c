/*
 * Calls a function before any declaration of it. The function exists
 * (defined below), so only the missing declaration can stop the build.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ST.c.014
 */
#include "pc.h"

int main(void)
{
	pc_begin("ST.c.014");
	pc_test();
	if (pc_selftest_late() != 12345)
		pc_fail("unreachable");
	pc_pass();
}

int pc_selftest_late(void)
{
	return 12345;
}
