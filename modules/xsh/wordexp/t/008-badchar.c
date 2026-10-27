/*
 * An unquoted newline, bar, ampersand, semicolon, less-than or greater-than makes wordexp fail with WRDE_BADCHAR and zero words.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.008
 */
#include "wx.h"

int main(void)
{
	static const char *const bad[] = {
		"a|b", "a&b", "a;b", "a<b", "a>b", "a\nb", "a | b", "a;", "|a", "a & b"
	};
	size_t i;

	pc_begin("XSH.wordexp.008");
	pc_test();
	for (i = 0; i < WX_N(bad); i++)
		wx_fails(bad[i], 0, WRDE_BADCHAR, WRDE_BADCHAR, 1);
	pc_pass();
}
