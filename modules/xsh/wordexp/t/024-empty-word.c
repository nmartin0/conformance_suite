/*
 * A quoted empty string is kept as an empty word.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.024
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "\"\"", 0, 1, { "" } },
	{ "''", 0, 1, { "" } },
	{ "a \"\" b", 0, 3, { "a", "", "b" } },
	{ "x\"\"", 0, 1, { "x" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.024");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
