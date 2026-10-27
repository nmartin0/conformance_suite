/*
 * WRDE_UNDEF: expanding an unset variable fails with WRDE_BADVAL; without it the variable is empty.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.014
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "$PCW_V", WRDE_UNDEF, 2, { "hello", "world" } },
	{ "$PCW_E", WRDE_UNDEF, 0, { 0 } },
	{ "\"$PCW_E\"", WRDE_UNDEF, 1, { "" } },
	{ "$PCW_U", 0, 0, { 0 } },
	{ "a${PCW_U}", 0, 1, { "a" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.014");
	pc_check(setenv("PCW_V", "hello world", 1) == 0, "setenv failed");
	pc_check(setenv("PCW_E", "", 1) == 0, "setenv failed");
	pc_check(unsetenv("PCW_U") == 0, "unsetenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	wx_fails("$PCW_U", WRDE_UNDEF, WRDE_BADVAL, WRDE_BADVAL, 0);
	wx_fails("a${PCW_U}", WRDE_UNDEF, WRDE_BADVAL, WRDE_BADVAL, 0);
	wx_fails("\"$PCW_U\"", WRDE_UNDEF, WRDE_BADVAL, WRDE_BADVAL, 0);
	wx_fails("x $PCW_U", WRDE_UNDEF, WRDE_BADVAL, WRDE_BADVAL, 0);
	pc_pass();
}
