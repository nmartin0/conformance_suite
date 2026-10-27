/*
 * Tilde expansion of an unquoted leading tilde uses HOME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.006
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "~", 0, 1, { "/pcw/home" } },
	{ "~/x", 0, 1, { "/pcw/home/x" } },
	{ "\"~\"", 0, 1, { "~" } },
	{ "'~'", 0, 1, { "~" } },
	{ "a~", 0, 1, { "a~" } },
	{ "~/a b", 0, 2, { "/pcw/home/a", "b" } },
	{ "x ~", 0, 2, { "x", "/pcw/home" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.006");
	pc_check(setenv("HOME", "/pcw/home", 1) == 0, "setenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
