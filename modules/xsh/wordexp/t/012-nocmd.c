/*
 * WRDE_NOCMD: a command substitution fails with WRDE_CMDSUB and the command is not run.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.012
 */
#include "wx.h"

#include <unistd.h>

int main(void)
{
	static const char *const bad[] = {
		"$(echo x)", "`echo x`", "\"$(echo x)\"", "a$(true)b", "$(echo x) y"
	};
	wordexp_t w;
	size_t i;

	pc_begin("XSH.wordexp.012");
	pc_test();
	for (i = 0; i < WX_N(bad); i++)
		wx_fails(bad[i], WRDE_NOCMD, WRDE_CMDSUB, WRDE_CMDSUB, 0);
	memset(&w, 0, sizeof w);
	pc_check(wordexp("$(touch pcw_nocmd_marker)", &w, WRDE_NOCMD) == WRDE_CMDSUB,
	    "the touch substitution was not refused");
	pc_check(access("pcw_nocmd_marker", F_OK) != 0, "the command was run despite WRDE_NOCMD");
	pc_pass();
}
