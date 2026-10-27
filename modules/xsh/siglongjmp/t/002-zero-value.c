/*
 * siglongjmp() with value 0 makes sigsetjmp() return 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.siglongjmp.002
 */
#include "pc.h"
#include <setjmp.h>

static sigjmp_buf env;

int main(void)
{
	volatile int mask, jumps;

	pc_begin("XSH.siglongjmp.002");
	pc_test();
	for (mask = 0; mask < 2; mask++) {
		jumps = 0;
		switch (sigsetjmp(env, mask)) {
		case 0:
			jumps++;
			siglongjmp(env, 0);
		case 1:
			break;
		default:
			pc_fail("savemask %d: sigsetjmp() returned another value than 1 after siglongjmp(env, 0)", mask);
		}
		pc_check(jumps == 1, "savemask %d: the direct path ran %d times", mask, jumps);
	}
	pc_pass();
}
