/*
 * NDEBUG removes assert().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.assert.004
 */
#define NDEBUG
#include "pc.h"
#include <assert.h>

int main(void)
{
	int evaluated = 0;

	pc_begin("XSH.assert.004");
	pc_test();
	assert(0);
	assert((evaluated = 1) == 2);
	pc_check(evaluated == 0, "the expression was evaluated although NDEBUG is defined");
	pc_pass();
}
