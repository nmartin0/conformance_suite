/*
 * sigsetjmp() returns 0 directly and non-zero after siglongjmp().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigsetjmp.001
 */
#include "pc.h"
#include <setjmp.h>

static sigjmp_buf env;

int main(void)
{
	volatile int direct = 0, jumped = 0, mask;

	pc_begin("XSH.sigsetjmp.001");
	pc_test();
	for (mask = 0; mask < 2; mask++) {
		direct = 0;
		jumped = 0;
		if (sigsetjmp(env, mask) == 0) {
			direct++;
			pc_check(direct == 1, "savemask %d: the direct return happened %d times", mask, direct);
			siglongjmp(env, 3);
		} else {
			jumped++;
			if (jumped < 3)
				siglongjmp(env, 4);
		}
		pc_check(direct == 1, "savemask %d: the direct return happened %d times", mask, direct);
		pc_check(jumped == 3, "savemask %d: sigsetjmp() returned %d times after siglongjmp(), expected 3", mask, jumped);
	}
	pc_pass();
}
