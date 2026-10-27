/*
 * Arithmetic expansion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.005
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "$((1+2))", 0, 1, { "3" } },
	{ "$((2*3+4))", 0, 1, { "10" } },
	{ "$((PCW_N+1))", 0, 1, { "42" } },
	{ "a$((7-2))b", 0, 1, { "a5b" } },
	{ "$((10/3))", 0, 1, { "3" } },
	{ "$((-5))", 0, 1, { "-5" } },
	{ "$((1+2)) $((3*3))", 0, 2, { "3", "9" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.005");
	pc_check(setenv("PCW_N", "41", 1) == 0, "setenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
