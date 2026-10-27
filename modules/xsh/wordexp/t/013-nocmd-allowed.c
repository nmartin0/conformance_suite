/*
 * WRDE_NOCMD does not hinder expansions that need no command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.013
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "a b", WRDE_NOCMD, 2, { "a", "b" } },
	{ "$PCW_V", WRDE_NOCMD, 2, { "hello", "world" } },
	{ "$((1+2))", WRDE_NOCMD, 1, { "3" } },
	{ "'$(echo x)'", WRDE_NOCMD, 1, { "$(echo x)" } },
	{ "\"\\$(echo x)\"", WRDE_NOCMD, 1, { "$(echo x)" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.013");
	pc_check(setenv("PCW_V", "hello world", 1) == 0, "setenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
