/*
 * REG_NEWLINE: a newline stays ordinary except that a period and a non-matching list do not match it and the anchors work at line boundaries.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.029
 */
#include "rx.h"

#define NL REG_NEWLINE
#define EX REG_EXTENDED
static const struct rxc tab[] = {
	/* without the flag a newline is an ordinary character */
	{ "a.b", 0, "a\nb", 0, 0, 3 },
	{ "a[^x]b", 0, "a\nb", 0, 0, 3 },
	{ "^b", 0, "a\nb", 0, -1, 0 },
	{ "a$", 0, "a\nb", 0, -1, 0 },
	{ "a\nb", 0, "a\nb", 0, 0, 3 },
	{ "a.b", EX, "a\nb", 0, 0, 3 },
	{ "^b", EX, "a\nb", 0, -1, 0 },
	{ "a$", EX, "a\nb", 0, -1, 0 },
	/* with the flag */
	{ "a.b", NL, "a\nb", 0, -1, 0 },
	{ "a[^x]b", NL, "a\nb", 0, -1, 0 },
	{ "a[\n]b", NL, "a\nb", 0, 0, 3 },
	{ "a\nb", NL, "a\nb", 0, 0, 3 },
	{ "^b", NL, "a\nb", 0, 2, 3 },
	{ "a$", NL, "a\nb", 0, 0, 1 },
	{ "^b", NL, "a\nb", REG_NOTBOL, 2, 3 },
	{ "a$", NL, "a\nb", REG_NOTEOL, 0, 1 },
	{ "^a", NL, "a\nb", REG_NOTBOL, -1, 0 },
	{ "^a$", NL, "x\na\ny", 0, 2, 3 },
	{ ".*", NL, "ab\ncd", 0, 0, 2 },
	{ "a.b", EX | NL, "a\nb", 0, -1, 0 },
	{ "a[^x]b", EX | NL, "a\nb", 0, -1, 0 },
	{ "^b", EX | NL, "a\nb", 0, 2, 3 },
	{ "a$", EX | NL, "a\nb", 0, 0, 1 },
	{ "(^b)", EX | NL, "a\nb", 0, 2, 3 },
};

int main(void)
{
	pc_begin("XSH.regcomp.029");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
