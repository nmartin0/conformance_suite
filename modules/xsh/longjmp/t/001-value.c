/*
 * longjmp() passes its value to setjmp().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.longjmp.001
 */
#include "pc.h"
#include <limits.h>
#include <setjmp.h>

static jmp_buf env;

/* setjmp() may appear only as a whole switch expression here, so each value gets its own case. */
#define TRY(V) \
	do { \
		switch (setjmp(env)) { \
		case 0: \
			longjmp(env, (V)); \
		case (V): \
			break; \
		default: \
			pc_fail("longjmp(env, %d) made setjmp() return another value", (V)); \
		} \
	} while (0)

int main(void)
{
	pc_begin("XSH.longjmp.001");
	pc_test();
	TRY(1);
	TRY(2);
	TRY(7);
	TRY(-5);
	TRY(INT_MAX);
	TRY(INT_MIN);
	pc_pass();
}
