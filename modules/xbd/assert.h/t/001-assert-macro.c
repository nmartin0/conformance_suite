/*
 * assert is a macro whose behaviour follows NDEBUG at each inclusion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.assert.h.001
 */
#undef NDEBUG
#include <assert.h>
#include "pc.h"

#ifndef assert
#error "assert is not a macro"
#endif

static int n;

static void active(void)
{
	assert(n++ == 0);
	assert(1);
}

#define NDEBUG
#include <assert.h>

static void disabled(void)
{
	assert(n++);
	assert(0);
	assert(0 && "never evaluated");
}

#undef NDEBUG
#include <assert.h>

static void active_again(void)
{
	assert(n++ == 1);
}

int main(void)
{
	pc_begin("XBD.assert.h.001");
	pc_test();
	active();
	pc_check(n == 1, "an active assert evaluated its expression %d times", n);
	disabled();
	pc_check(n == 1, "a disabled assert evaluated its expression (n is %d)", n);
	active_again();
	pc_check(n == 2, "assert did not become active again after NDEBUG was undefined and <assert.h> included (n is %d)", n);
	pc_pass();
}
