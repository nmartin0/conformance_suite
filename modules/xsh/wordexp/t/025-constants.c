/*
 * The WRDE_ flag and error constants are nonzero and distinct, and the flags do not share bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.025
 */
#include "wx.h"

int main(void)
{
	static const int flags[] = {
		WRDE_APPEND, WRDE_DOOFFS, WRDE_NOCMD, WRDE_REUSE, WRDE_SHOWERR, WRDE_UNDEF
	};
	static const int errs[] = {
		WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_NOSPACE, WRDE_SYNTAX
	};
	wordexp_t w;
	size_t i, j;

	pc_begin("XSH.wordexp.025");
	pc_test();
	for (i = 0; i < WX_N(flags); i++) {
		pc_check(flags[i] != 0, "flag %zu is zero", i);
		for (j = i + 1; j < WX_N(flags); j++)
			pc_check((flags[i] & flags[j]) == 0, "flags %zu and %zu overlap", i, j);
	}
	for (i = 0; i < WX_N(errs); i++) {
		pc_check(errs[i] != 0, "error constant %zu is zero", i);
		for (j = i + 1; j < WX_N(errs); j++)
			pc_check(errs[i] != errs[j], "error constants %zu and %zu are equal", i, j);
	}
	pc_check(_Generic(w.we_wordc, size_t: 1, default: 0), "we_wordc is not a size_t");
	pc_check(_Generic(w.we_offs, size_t: 1, default: 0), "we_offs is not a size_t");
	pc_check(_Generic(w.we_wordv, char **: 1, default: 0), "we_wordv is not a char **");
	pc_pass();
}
