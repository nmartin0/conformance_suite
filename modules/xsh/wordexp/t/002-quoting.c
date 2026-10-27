/*
 * Quoting is honoured and quote removal leaves the quoted text.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.002
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "'a b'", 0, 1, { "a b" } },
	{ "\"a b\"", 0, 1, { "a b" } },
	{ "a\\ b", 0, 1, { "a b" } },
	{ "\"a'b\"", 0, 1, { "a'b" } },
	{ "'a\"b'", 0, 1, { "a\"b" } },
	{ "a\"b c\"d", 0, 1, { "ab cd" } },
	{ "\"a\\\"b\"", 0, 1, { "a\"b" } },
	{ "'\\n'", 0, 1, { "\\n" } },
	{ "\"a\\\\b\"", 0, 1, { "a\\b" } },
	{ "a''b", 0, 1, { "ab" } },
	{ "x'  'y", 0, 1, { "x  y" } },
	{ "\"\\a\"", 0, 1, { "\\a" } },
	{ "'a'\"b\"c", 0, 1, { "abc" } },
	{ "a\\'b", 0, 1, { "a'b" } },
	{ "\\\\", 0, 1, { "\\" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.002");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
