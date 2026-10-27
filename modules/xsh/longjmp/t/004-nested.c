/*
 * longjmp() out of nested calls.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.longjmp.004
 */
#include "pc.h"
#include <setjmp.h>

static jmp_buf env;

static void descend(int depth, int val)
{
	volatile char pad[64];

	pad[0] = (char)depth;
	if (depth > 0)
		descend(depth - 1, val);
	else
		longjmp(env, val);
	pad[1] = pad[0];
}

int main(void)
{
	volatile int i, sum = 0, got;

	pc_begin("XSH.longjmp.004");
	pc_test();
	for (i = 0; i < 1000; i++) {
		got = -1;
		switch (setjmp(env)) {
		case 0:
			descend(50, 1 + i % 7);
			break;
		case 1: got = 1; break;
		case 2: got = 2; break;
		case 3: got = 3; break;
		case 4: got = 4; break;
		case 5: got = 5; break;
		case 6: got = 6; break;
		case 7: got = 7; break;
		default: break;
		}
		pc_check(got == 1 + i % 7, "round %d: setjmp() returned %d, expected %d", i, got, 1 + i % 7);
		sum += got;
	}
	pc_check(i == 1000, "the loop variable is %d after the jumps", i);
	pc_check(sum > 0, "the sum is %d", sum);
	pc_pass();
}
