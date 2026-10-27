/*
 * siglongjmp() passes its value to sigsetjmp().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.siglongjmp.001
 */
#include "pc.h"
#include <limits.h>
#include <setjmp.h>

static sigjmp_buf env;

/* sigsetjmp() may appear only as a whole switch expression here, so each value gets its own case. */
#define TRY(V, M) \
	do { \
		switch (sigsetjmp(env, (M))) { \
		case 0: \
			siglongjmp(env, (V)); \
		case (V): \
			break; \
		default: \
			pc_fail("siglongjmp(env, %d) made sigsetjmp() return another value (savemask %d)", (V), (M)); \
		} \
	} while (0)

int main(void)
{
	pc_begin("XSH.siglongjmp.001");
	pc_test();
	TRY(1, 0); TRY(2, 0); TRY(7, 0); TRY(-5, 0); TRY(INT_MAX, 0); TRY(INT_MIN, 0);
	TRY(1, 1); TRY(2, 1); TRY(7, 1); TRY(-5, 1); TRY(INT_MAX, 1); TRY(INT_MIN, 1);
	pc_pass();
}
