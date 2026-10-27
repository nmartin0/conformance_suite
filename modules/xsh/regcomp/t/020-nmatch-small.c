/*
 * With fewer elements requested than subexpressions the match is still decided in full and only the requested elements are written.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.020
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "(a)(b)(c)", REG_EXTENDED, "abc", 0, 2, { 0, 3, 0, 1 } },
	{ "(a)(b)(c)", REG_EXTENDED, "abc", 0, 1, { 0, 3 } },
	{ "\\(a\\)\\(b\\)\\(c\\)", 0, "xabcx", 0, 3, { 1, 4, 1, 2, 2, 3 } },
};

int main(void)
{
	regex_t re;
	regmatch_t m[3];
	int k;

	pc_begin("XSH.regcomp.020");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_check(regcomp(&re, "(a)(b)(c)", REG_EXTENDED) == 0, "regcomp failed");
	for (k = 0; k < 3; k++)
		m[k].rm_so = m[k].rm_eo = RX_SENT;
	pc_check(regexec(&re, "abd", 2, m, 0) == REG_NOMATCH,
	    "a string failing on the third subexpression matched with nmatch 2");
	regfree(&re);
	pc_pass();
}
