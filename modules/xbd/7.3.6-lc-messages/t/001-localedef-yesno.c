/*
 * The yesexpr and noexpr extended regular expressions given to localedef
 * are the ones nl_langinfo(YESEXPR) and nl_langinfo(NOEXPR) return, and
 * compiled as extended expressions they match the intended affirmative
 * and negative answers (here j and J, k and K) and not the POSIX
 * locale's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.6-lc-messages.001
 */
#include "pc.h"
#include "ldef.h"
#include <langinfo.h>
#include <regex.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME

static int matches(const char *expr, const char *s)
{
	regex_t re;
	int r;

	if (regcomp(&re, expr, REG_EXTENDED | REG_NOSUB) != 0)
		return -1;
	r = regexec(&re, s, 0, NULL, 0) == 0;
	regfree(&re);
	return r;
}

int main(void)
{
	char loc[4200], yes[300], no[300], text[1500];
	const char *y, *n;

	pc_begin("XBD.7.3.6-lc-messages.001");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	(void)snprintf(text, sizeof text, "LC_MESSAGES\nyesexpr %s\nnoexpr %s\nEND LC_MESSAGES\n%s",
	    pcc_str(yes, sizeof yes, "^([jJ])"), pcc_str(no, sizeof no, "^([kK])"), REST);
	ld_ok("msg", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	y = nl_langinfo(YESEXPR);
	n = nl_langinfo(NOEXPR);
	pc_check(y != NULL && strcmp(y, "^([jJ])") == 0, "YESEXPR is \"%s\", not ^([jJ])", y ? y : "(null)");
	pc_check(n != NULL && strcmp(n, "^([kK])") == 0, "NOEXPR is \"%s\", not ^([kK])", n ? n : "(null)");
	if (y != NULL && n != NULL) {
		pc_check(matches(y, "j") == 1 && matches(y, "J") == 1 && matches(y, "jawohl") == 1, "YESEXPR does not match j, J or jawohl");
		pc_check(matches(y, "k") == 0 && matches(y, "y") == 0 && matches(y, "n") == 0, "YESEXPR matches k, y or n");
		pc_check(matches(n, "k") == 1 && matches(n, "K") == 1 && matches(n, "kein") == 1, "NOEXPR does not match k, K or kein");
		pc_check(matches(n, "j") == 0 && matches(n, "n") == 0 && matches(n, "y") == 0, "NOEXPR matches j, n or y");
	}
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
