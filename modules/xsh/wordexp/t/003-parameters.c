/*
 * Parameter expansion, with field splitting of unquoted results.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.003
 */
#include "wx.h"

static const struct wxc tab[] = {
	{ "$PCW_V", 0, 2, { "hello", "world" } },
	{ "\"$PCW_V\"", 0, 1, { "hello world" } },
	{ "${PCW_V}x", 0, 2, { "hello", "worldx" } },
	{ "${PCW_U:-dflt}", 0, 1, { "dflt" } },
	{ "${PCW_U-dflt}", 0, 1, { "dflt" } },
	{ "${PCW_E:-d}", 0, 1, { "d" } },
	{ "${PCW_E-d}", 0, 0, { 0 } },
	{ "${#PCW_V}", 0, 1, { "11" } },
	{ "a${PCW_U}b", 0, 1, { "ab" } },
	{ "${PCW_V%% *}", 0, 1, { "hello" } },
	{ "${PCW_V#* }", 0, 1, { "world" } },
	{ "$PCW_V$PCW_V", 0, 3, { "hello", "worldhello", "world" } },
	{ "'$PCW_V'", 0, 1, { "$PCW_V" } },
	{ "\"${PCW_V}\"x", 0, 1, { "hello worldx" } },
	{ "\\$PCW_V", 0, 1, { "$PCW_V" } },
};

int main(void)
{
	pc_begin("XSH.wordexp.003");
	pc_check(setenv("PCW_V", "hello world", 1) == 0, "setenv failed");
	pc_check(setenv("PCW_E", "", 1) == 0, "setenv failed");
	pc_check(unsetenv("PCW_U") == 0, "unsetenv failed");
	pc_test();
	wx_cases(tab, WX_N(tab));
	pc_pass();
}
