/*
 * WRDE_DOOFFS places we_offs null pointers before the words; we_wordc does not count them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.017
 */
#include "wx.h"

int main(void)
{
	static const char *const v2[] = { "x", "y" };
	wordexp_t w;
	int r;

	pc_begin("XSH.wordexp.017");
	pc_check(setenv("PCW_E", "", 1) == 0, "setenv failed");
	pc_test();
	memset(&w, 0, sizeof w);
	w.we_offs = 3;
	r = wordexp("x y", &w, WRDE_DOOFFS);
	pc_check(r == 0, "wordexp returned %d", r);
	wx_check(&w, 3, 2, v2, "offset 3");
	wordfree(&w);

	memset(&w, 0, sizeof w);
	w.we_offs = 0;
	r = wordexp("x y", &w, WRDE_DOOFFS);
	pc_check(r == 0, "wordexp returned %d", r);
	wx_check(&w, 0, 2, v2, "offset 0");
	wordfree(&w);

	memset(&w, 0, sizeof w);
	w.we_offs = 2;
	r = wordexp("$PCW_E", &w, WRDE_DOOFFS);
	pc_check(r == 0, "wordexp returned %d", r);
	wx_check(&w, 2, 0, NULL, "offset 2, no words");
	wordfree(&w);
	pc_pass();
}
