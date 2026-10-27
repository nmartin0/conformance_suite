/*
 * An unterminated quote is a syntax error: WRDE_SYNTAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.011
 */
#include "wx.h"

int main(void)
{
	static const char *const bad[] = { "\"abc", "'abc", "a \"b c", "a 'b c" };
	size_t i;

	pc_begin("XSH.wordexp.011");
	pc_test();
	for (i = 0; i < WX_N(bad); i++)
		wx_fails(bad[i], 0, WRDE_SYNTAX, WRDE_SYNTAX, 0);
	pc_pass();
}
