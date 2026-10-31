/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <stddef.h>
int main(void)
{
pc_begin("ST.c.028"); pc_make_file("f", "abc"); pc_test();
	/* a present file with other contents is an answer (0), not a failed observation */
	if (pc_file_equals("f", "xyz") != 0)
		pc_fail("mismatch not reported as 0");
	pc_pass();
}
