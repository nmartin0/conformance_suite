/*
 * The POSIX collation sequence has no multi-character collating elements:
 * a bracket expression holding the collating symbol of two characters is
 * not valid in the POSIX locale, while one holding a single character is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.003
 */
#include "pc.h"
#include <locale.h>
#include <regex.h>

int main(void)
{
	regex_t re;
	int r;

	pc_begin("XBD.7.3.2-lc-collate.003");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	r = regcomp(&re, "[[.a.]]", REG_EXTENDED);
	pc_check(r == 0, "regcomp of a bracket expression with the collating symbol of one character failed (%d)", r);
	regfree(&re);
	r = regcomp(&re, "[[=a=]]", REG_EXTENDED);
	pc_check(r == 0, "regcomp of a bracket expression with an equivalence class failed (%d)", r);
	regfree(&re);
	r = regcomp(&re, "[[.ab.]]", REG_EXTENDED);
	if (r == 0)
		regfree(&re);
	pc_check(r != 0, "regcomp accepted the collating symbol \"ab\" although the POSIX locale has no such collating element");
	r = regcomp(&re, "[[.ab.]]", 0);
	if (r == 0)
		regfree(&re);
	pc_check(r != 0, "regcomp (basic) accepted the collating symbol \"ab\" although the POSIX locale has no such collating element");
	pc_pass();
}
