/*
 * REG_NOTBOL: the first character of the string is not the beginning of a line.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.030
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "^a", 0, "a", REG_NOTBOL, -1, 0 },
	{ "^a", 0, "a", 0, 0, 1 },
	{ "^a", REG_EXTENDED, "a", REG_NOTBOL, -1, 0 },
	{ "^a|b", REG_EXTENDED, "ab", REG_NOTBOL, 1, 2 },
	{ "a", 0, "a", REG_NOTBOL, 0, 1 },
	{ "a^b", 0, "a^b", REG_NOTBOL, 0, 3 },
};

int main(void)
{
	regex_t re;
	regmatch_t pm;
	const char *buf;
	int count, flags, off;

	pc_begin("XSH.regcomp.030");
	pc_test();
	rx_cases(tab, RX_N(tab));

	/* the idiom of finding every match on a line */
	pc_check(regcomp(&re, "an", 0) == 0, "regcomp failed");
	buf = "banana";
	count = 0;
	off = 0;
	flags = 0;
	while (regexec(&re, buf + off, 1, &pm, flags) == 0 && count < 10) {
		count++;
		off += (int)pm.rm_eo;
		flags = REG_NOTBOL;
	}
	pc_check(count == 2, "found %d matches of \"an\" in \"banana\", expected 2", count);
	regfree(&re);

	pc_check(regcomp(&re, "^a", 0) == 0, "regcomp failed");
	buf = "aaa";
	count = 0;
	off = 0;
	flags = 0;
	while (regexec(&re, buf + off, 1, &pm, flags) == 0 && count < 10) {
		count++;
		off += (int)pm.rm_eo;
		flags = REG_NOTBOL;
	}
	pc_check(count == 1, "found %d matches of \"^a\" in \"aaa\", expected 1", count);
	regfree(&re);
	pc_pass();
}
