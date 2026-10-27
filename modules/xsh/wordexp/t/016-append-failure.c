/*
 * A failing wordexp with WRDE_APPEND leaves we_wordc and we_wordv unmodified.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.016
 */
#include "wx.h"

int main(void)
{
	static const char *const v[] = { "a", "b" };
	wordexp_t w;
	char **save;
	size_t cnt;
	int r;

	pc_begin("XSH.wordexp.016");
	pc_test();
	memset(&w, 0, sizeof w);
	r = wordexp("a b", &w, 0);
	pc_check(r == 0, "first wordexp returned %d", r);
	save = w.we_wordv;
	cnt = w.we_wordc;

	r = wordexp("c|d", &w, WRDE_APPEND);
	pc_check(r == WRDE_BADCHAR, "append of c|d returned %d", r);
	pc_check(w.we_wordc == cnt && w.we_wordv == save, "BADCHAR changed we_wordc or we_wordv");
	wx_check(&w, 0, 2, v, "after the BADCHAR failure");

	r = wordexp("\"c d", &w, WRDE_APPEND);
	pc_check(r == WRDE_SYNTAX, "append of an unterminated quote returned %d", r);
	pc_check(w.we_wordc == cnt && w.we_wordv == save, "SYNTAX changed we_wordc or we_wordv");
	wx_check(&w, 0, 2, v, "after the SYNTAX failure");

	r = wordexp("$(echo z)", &w, WRDE_APPEND | WRDE_NOCMD);
	pc_check(r == WRDE_CMDSUB, "append of a command substitution returned %d", r);
	pc_check(w.we_wordc == cnt && w.we_wordv == save, "CMDSUB changed we_wordc or we_wordv");
	wx_check(&w, 0, 2, v, "after the CMDSUB failure");
	wordfree(&w);
	pc_pass();
}
