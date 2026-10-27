/*
 * Words are separated at blanks; the list ends with a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.001
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "a b  c", 0, 3, { "a", "b", "c" } },
	{ "  lead trail  ", 0, 2, { "lead", "trail" } },
	{ "x\ty", 0, 2, { "x", "y" } },
	{ "single", 0, 1, { "single" } },
	{ "a\tb  c d", 0, 4, { "a", "b", "c", "d" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.001");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
