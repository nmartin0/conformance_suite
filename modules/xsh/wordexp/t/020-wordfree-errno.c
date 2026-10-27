/*
 * wordfree does not change errno for a structure filled by wordexp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.020
 */
#include "wx.h"

int main(void)
{
	wordexp_t w;
	int r;

	pc_begin("XSH.wordexp.020");
	pc_test();
	memset(&w, 0, sizeof w);
	r = wordexp("a b c", &w, 0);
	pc_check(r == 0, "wordexp returned %d", r);
	errno = EDOM;
	wordfree(&w);
	pc_check(errno == EDOM, "wordfree changed errno to %d", errno);
	memset(&w, 0, sizeof w);
	w.we_offs = 2;
	r = wordexp("x", &w, WRDE_DOOFFS);
	pc_check(r == 0, "wordexp returned %d", r);
	errno = ERANGE;
	wordfree(&w);
	pc_check(errno == ERANGE, "wordfree changed errno to %d", errno);
	pc_pass();
}
