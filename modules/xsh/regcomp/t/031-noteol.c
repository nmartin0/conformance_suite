/*
 * REG_NOTEOL: the last character of the string is not the end of a line.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.031
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "a$", 0, "a", REG_NOTEOL, -1, 0 },
	{ "a$", 0, "a", 0, 0, 1 },
	{ "a$", REG_EXTENDED, "a", REG_NOTEOL, -1, 0 },
	{ "a$|b", REG_EXTENDED, "ab", REG_NOTEOL, 1, 2 },
	{ "^a$", 0, "a", REG_NOTBOL | REG_NOTEOL, -1, 0 },
	{ "a", 0, "a", REG_NOTBOL | REG_NOTEOL, 0, 1 },
	{ "a$b", 0, "a$b", REG_NOTEOL, 0, 3 },
};

int main(void)
{
	pc_begin("XSH.regcomp.031");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
