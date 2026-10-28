/*
 * ERE precedence and multiple alternatives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.005
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	{ "ab+", EX, "abbbab", 0, 0, 4 },
	{ "ab{2}", EX, "ababb", 0, 2, 5 },
	{ "ab{2}", EX, "abab", 0, -1, 0 },
	{ "(ab){2}", EX, "ababab", 0, 0, 4 },
	{ "^a|b$", EX, "ax", 0, 0, 1 },
	{ "^a|b$", EX, "xb", 0, 1, 2 },
	{ "^a|b$", EX, "xax", 0, -1, 0 },
	{ "^a|b$", EX, "bx", 0, -1, 0 },
	{ "^a|b$", EX, "xab", 0, 2, 3 },
	{ "ab|cd", EX, "xcd", 0, 1, 3 },
	{ "a|b|c", EX, "xc", 0, 1, 2 },
	{ "a|ab|abc", EX, "abcd", 0, 0, 3 },
	{ "^ab|cd$", EX, "abxcd", 0, 0, 2 },
	{ "^ab|cd$", EX, "xabcd", 0, 3, 5 },
	{ "^ab|cd$", EX, "xabxx", 0, -1, 0 },
	{ "(a|b)c|d", EX, "xd", 0, 1, 2 },
	{ "(a|b)c|d", EX, "ac", 0, 0, 2 },
	{ "(a|b)c|d", EX, "bd", 0, 1, 2 },
	{ "a|b*", EX, "xbb", 0, 0, 0 },
	{ "a|b*", EX, "bbb", 0, 0, 3 },
	{ "a\\.*b", EX, "a...b", 0, 0, 5 },
	{ "\\[a]", EX, "[a]", 0, 0, 3 },
	{ "\\[a]", EX, "a]", 0, -1, 0 },
};
static const struct rxs subs[] = {
	{ "a(b|c)+d", EX, "xabcbd", 0, 2, { 1, 6, 4, 5 } },
};

int main(void)
{
	pc_begin("XBD.9.4-ere.005");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
