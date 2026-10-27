/*
 * WRDE_SHOWERR is accepted and expansions still work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.022
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "$(echo hi)", WRDE_SHOWERR, 1, { "hi" } },
	{ "a b", WRDE_SHOWERR, 2, { "a", "b" } },
	{ "$PCW_V", WRDE_SHOWERR | WRDE_UNDEF, 2, { "hello", "world" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.022");
	pc_check(setenv("PCW_V", "hello world", 1) == 0, "setenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
