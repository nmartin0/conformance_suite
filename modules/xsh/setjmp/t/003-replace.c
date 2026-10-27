/*
 * A second setjmp() with the same buffer replaces the first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setjmp.003
 */
#include "pc.h"
#include <setjmp.h>

static jmp_buf env;

int main(void)
{
	volatile int first = 0, second = 0, after = 0;

	pc_begin("XSH.setjmp.003");
	pc_test();
	if (setjmp(env) == 0)
		first++;
	if (setjmp(env) == 0) {
		second++;
		pc_check(second == 1 && first == 1, "longjmp() resumed at the first setjmp() (first %d, second %d)", first, second);
		longjmp(env, 1);
	} else
		after++;
	pc_check(first == 1, "longjmp() resumed at the first setjmp() (it ran %d times)", first);
	pc_check(second == 1, "the second setjmp() returned directly %d times", second);
	pc_check(after == 1, "the return from longjmp() happened %d times", after);
	pc_pass();
}
