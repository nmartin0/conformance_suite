/*
 * WRDE_DOOFFS with WRDE_APPEND keeps the leading null pointers and appends in order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.018
 */
#include "wx.h"

int main(void)
{
	static const char *const v[] = { "a", "b", "c" };
	wordexp_t w;
	int r;

	pc_begin("XSH.wordexp.018");
	pc_test();
	memset(&w, 0, sizeof w);
	w.we_offs = 2;
	r = wordexp("a b", &w, WRDE_DOOFFS);
	pc_check(r == 0, "first wordexp returned %d", r);
	r = wordexp("c", &w, WRDE_DOOFFS | WRDE_APPEND);
	pc_check(r == 0, "second wordexp returned %d", r);
	wx_check(&w, 2, 3, v, "offset 2 with append");
	wordfree(&w);
	pc_pass();
}
