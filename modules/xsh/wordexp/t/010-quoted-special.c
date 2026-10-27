/*
 * The special characters are accepted when quoted or escaped.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.010
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "'a|b'", 0, 1, { "a|b" } },
	{ "\"a;b\"", 0, 1, { "a;b" } },
	{ "a\\|b", 0, 1, { "a|b" } },
	{ "'a b(c)'", 0, 1, { "a b(c)" } },
	{ "\"{x}\"", 0, 1, { "{x}" } },
	{ "'a&b<c>d'", 0, 1, { "a&b<c>d" } },
	{ "a\\;b", 0, 1, { "a;b" } },
	{ "\"a\nb\"", 0, 1, { "a\nb" } },
	{ "a\\(b\\)", 0, 1, { "a(b)" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.010");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
