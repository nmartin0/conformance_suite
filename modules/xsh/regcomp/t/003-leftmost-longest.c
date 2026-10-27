/*
 * The match starts earliest in the string and, from there, is the longest.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.003
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "(wee|week)(knights|night)", REG_EXTENDED, "weeknights", 0, 0, 10 },
	{ "abba|cde", REG_EXTENDED, "abbcde", 0, 3, 6 },
	{ "abba|cde", REG_EXTENDED, "xcdeabba", 0, 1, 4 },
	{ "a|ab", REG_EXTENDED, "abc", 0, 0, 2 },
	{ "ab|abcd", REG_EXTENDED, "abcde", 0, 0, 4 },
	{ "(a|ab)(c|bcd)", REG_EXTENDED, "abcd", 0, 0, 4 },
	{ "x*", REG_EXTENDED, "axx", 0, 0, 0 },
	{ "bb*", 0, "abbbc", 0, 1, 4 },
	{ "a*b", 0, "aaacaab", 0, 4, 7 },
};
static const struct rxs subs[] = {
	{ "(wee|week)(knights|night)", REG_EXTENDED, "weeknights", 0, 3,
	  { 0, 10, 0, 3, 3, 10 } },
	{ "(a|ab)(c|bcd)", REG_EXTENDED, "abcd", 0, 3,
	  { 0, 4, 0, 1, 1, 4 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.003");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
