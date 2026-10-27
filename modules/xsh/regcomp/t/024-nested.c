/*
 * A subexpression inside a non-participating one is -1; inside a participating one it is judged within that one's last substring.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.024
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "((a)b)?c", REG_EXTENDED, "c", 0, 3, { 0, 1, -1, -1, -1, -1 } },
	{ "(a(b)?c)", REG_EXTENDED, "ac", 0, 3, { 0, 2, 0, 2, -1, -1 } },
	{ "(a(b)?c)", REG_EXTENDED, "abc", 0, 3, { 0, 3, 0, 3, 1, 2 } },
	{ "(a(b)*)*", REG_EXTENDED, "aba", 0, 3, { 0, 3, 2, 3, -1, -1 } },
	{ "\\(a\\(b\\)*\\)*", 0, "aba", 0, 3, { 0, 3, 2, 3, -1, -1 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.024");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
