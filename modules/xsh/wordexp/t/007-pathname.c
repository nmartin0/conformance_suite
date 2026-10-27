/*
 * Pathname expansion yields one word per file, sorted; an unmatched or quoted pattern is left alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.007
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "*.txt", 0, 3, { "a1.txt", "a2.txt", "b1.txt" } },
	{ "a?.txt", 0, 2, { "a1.txt", "a2.txt" } },
	{ "[ab]1.txt", 0, 2, { "a1.txt", "b1.txt" } },
	{ "'*.txt'", 0, 1, { "*.txt" } },
	{ "\"*.txt\"", 0, 1, { "*.txt" } },
	{ "\\*.txt", 0, 1, { "*.txt" } },
	{ "*.zzz", 0, 1, { "*.zzz" } },
	{ "x *.dat", 0, 2, { "x", "c.dat" } },
	{ "*", 0, 4, { "a1.txt", "a2.txt", "b1.txt", "c.dat" } },
};

int main(void)
{
	static const char *const names[] = { "a1.txt", "a2.txt", "b1.txt", "c.dat" };
	char path[64];
	size_t i;

	pc_begin("XSH.wordexp.007");
	pc_make_dir("wxd");
	for (i = 0; i < WX_N(names); i++) {
		snprintf(path, sizeof path, "wxd/%s", names[i]);
		pc_make_file(path, "x");
	}
	pc_check(chdir("wxd") == 0, "chdir failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_check(chdir("..") == 0, "chdir back failed");
	for (i = 0; i < WX_N(names); i++) {
		snprintf(path, sizeof path, "wxd/%s", names[i]);
		(void)unlink(path);
	}
	(void)rmdir("wxd");
	pc_pass();
}
