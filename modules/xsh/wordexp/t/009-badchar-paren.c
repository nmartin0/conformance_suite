/*
 * Unquoted parentheses and braces in an inappropriate place make wordexp fail with no words.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.009
 */
#include "wx.h"

int main(void)
{
	static const char *const bad[] = { "(a)", "a(b", "a)b", "{a", "a}", "a (b)" };
	size_t i;

	pc_begin("XSH.wordexp.009");
	pc_test();
	for (i = 0; i < WX_N(bad); i++)
		wx_fails(bad[i], 0, WRDE_BADCHAR, WRDE_SYNTAX, 1);
	pc_pass();
}
