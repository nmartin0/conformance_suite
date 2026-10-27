/*
 * setjmp() returns 0 directly and non-zero after longjmp().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setjmp.001
 */
#include "pc.h"
#include <setjmp.h>

static jmp_buf env;

int main(void)
{
	volatile int direct = 0, jumped = 0;

	pc_begin("XSH.setjmp.001");
	pc_test();
	if (setjmp(env) == 0) {
		direct++;
		pc_check(direct == 1, "the direct return happened %d times", direct);
		longjmp(env, 3);
	} else {
		jumped++;
		if (jumped < 3 && direct < 5) {
			longjmp(env, 4);
		}
	}
	pc_check(direct == 1, "the direct return happened %d times", direct);
	pc_check(jumped == 3, "setjmp() returned %d times after longjmp(), expected 3", jumped);
	pc_pass();
}
