/*
 * WRDE_REUSE gives the same result as wordfree followed by a fresh wordexp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.019
 */
#include "wx.h"

int main(void)
{
	static const char *const d[] = { "d" };
	static const char *const ef[] = { "e", "f" };
	static const char *const c[] = { "c" };
	wordexp_t w;
	int r;

	pc_begin("XSH.wordexp.019");
	pc_test();
	memset(&w, 0, sizeof w);
	r = wordexp("a b c", &w, 0);
	pc_check(r == 0, "first wordexp returned %d", r);
	r = wordexp("d", &w, WRDE_REUSE);
	pc_check(r == 0, "reuse wordexp returned %d", r);
	wx_check(&w, 0, 1, d, "after reuse");
	r = wordexp("e f", &w, WRDE_REUSE);
	pc_check(r == 0, "second reuse wordexp returned %d", r);
	wx_check(&w, 0, 2, ef, "after second reuse");
	wordfree(&w);

	memset(&w, 0, sizeof w);
	w.we_offs = 1;
	r = wordexp("a b", &w, WRDE_DOOFFS);
	pc_check(r == 0, "offset wordexp returned %d", r);
	r = wordexp("c", &w, WRDE_DOOFFS | WRDE_REUSE);
	pc_check(r == 0, "offset reuse returned %d", r);
	wx_check(&w, 1, 1, c, "offset 1 after reuse");
	wordfree(&w);
	pc_pass();
}
