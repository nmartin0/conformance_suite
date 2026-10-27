/*
 * An unquoted number sign beginning a token is a comment or an ordinary character; elsewhere it is ordinary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.023
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "a#b", 0, 1, { "a#b" } },
	{ "'#x'", 0, 1, { "#x" } },
	{ "\\#x", 0, 1, { "#x" } },
	{ "a '#b'", 0, 2, { "a", "#b" } },
	{ "\"#q\"", 0, 1, { "#q" } },
};

int main(void)
{
	wordexp_t w;
	int r;

	pc_begin("XSH.wordexp.023");
	pc_test();
	wx_cases(tab, WX_N(tab));
	memset(&w, 0, sizeof w);
	r = wordexp("a #b", &w, 0);
	pc_check(r == 0, "wordexp(\"a #b\") returned %d", r);
	pc_check(w.we_wordc == 1 || w.we_wordc == 2, "we_wordc is %zu", (size_t)w.we_wordc);
	pc_check(strcmp(w.we_wordv[0], "a") == 0, "first word is \"%s\"", w.we_wordv[0]);
	if (w.we_wordc == 2)
		pc_check(strcmp(w.we_wordv[1], "#b") == 0, "second word is \"%s\"", w.we_wordv[1]);
	pc_check(w.we_wordv[w.we_wordc] == NULL, "list not null-terminated");
	wordfree(&w);
	pc_pass();
}
