/*
 * longjmp() with value 0 makes setjmp() return 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.longjmp.002
 */
#include "pc.h"
#include <setjmp.h>

static jmp_buf env;

int main(void)
{
	volatile int jumps = 0;

	pc_begin("XSH.longjmp.002");
	pc_test();
	switch (setjmp(env)) {
	case 0:
		jumps++;
		longjmp(env, 0);
	case 1:
		break;
	default:
		pc_fail("setjmp() returned another value than 1 after longjmp(env, 0)");
	}
	pc_check(jumps == 1, "the direct path ran %d times", jumps);
	pc_pass();
}
