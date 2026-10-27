/*
 * WRDE_APPEND adds the new words after the earlier ones and we_wordc is the total.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.015
 */
#include "wx.h"

int main(void)
{
	static const char *const v3[] = { "a", "b", "c" };
	static const char *const v5[] = { "a", "b", "c", "d", "e" };
	static const char *const v6[] = { "a", "b", "c", "d", "e", "f" };
	wordexp_t w;
	int r;

	pc_begin("XSH.wordexp.015");
	pc_test();
	memset(&w, 0, sizeof w);
	r = wordexp("a b", &w, 0);
	pc_check(r == 0, "first wordexp returned %d", r);
	r = wordexp("c", &w, WRDE_APPEND);
	pc_check(r == 0, "second wordexp returned %d", r);
	wx_check(&w, 0, 3, v3, "after one append");
	r = wordexp("d e", &w, WRDE_APPEND);
	pc_check(r == 0, "third wordexp returned %d", r);
	wx_check(&w, 0, 5, v5, "after two appends");
	r = wordexp("f", &w, WRDE_APPEND);
	pc_check(r == 0, "fourth wordexp returned %d", r);
	wx_check(&w, 0, 6, v6, "after three appends");
	wordfree(&w);
	pc_pass();
}
