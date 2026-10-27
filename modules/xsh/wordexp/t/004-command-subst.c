/*
 * Command substitution in both notations, quoted and unquoted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.004
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "$(echo hi)", 0, 1, { "hi" } },
	{ "$(echo a b)", 0, 2, { "a", "b" } },
	{ "\"$(echo a b)\"", 0, 1, { "a b" } },
	{ "`echo x y`", 0, 2, { "x", "y" } },
	{ "a$(echo b)c", 0, 1, { "abc" } },
	{ "$(echo a; echo b)", 0, 2, { "a", "b" } },
	{ "$(echo  a   b)", 0, 2, { "a", "b" } },
	{ "\"$(echo a; echo b)\"", 0, 1, { "a\nb" } },
	{ "$(echo $(echo n))", 0, 1, { "n" } },
	{ "$(echo $PCW_V)", 0, 2, { "hello", "world" } },
	{ "\"`echo q`\"", 0, 1, { "q" } },
	{ "$(echo a) $(echo b)", 0, 2, { "a", "b" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.004");
	pc_check(setenv("PCW_V", "hello world", 1) == 0, "setenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
