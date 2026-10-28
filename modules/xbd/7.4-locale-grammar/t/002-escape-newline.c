/*
 * An escape character at the end of a line joins the line with the next:
 * a grouping list and a name list split after a semicolon read as if
 * written on one line.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.4-locale-grammar.002
 */
#include "pc.h"
#include "ldef.h"
#include <time.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_MESSAGES

int main(void)
{
	static char text[8000];
	char loc[4200], buf[100];
	const struct lconv *lc;
	struct tm t;

	pc_begin("XBD.7.4-locale-grammar.002");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	(void)snprintf(text, sizeof text,
	    "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3;\\\n2\nEND LC_NUMERIC\n"
	    "LC_TIME\nabday \"<u><a>\";\\\n\"<u><b>\";\"<u><c>\";\\\n  \"<u><d>\";\"<u><e>\";\"<u><f>\";\"<u><g>\"\n"
	    "day \"<u><a><d>\";\"<u><b><d>\";\"<u><c><d>\";\"<u><d><d>\";\"<u><e><d>\";\"<u><f><d>\";\"<u><g><d>\"\n"
	    "abmon \"<m><a>\";\"<m><b>\";\"<m><c>\";\"<m><d>\";\"<m><e>\";\"<m><f>\";\"<m><g>\";\"<m><h>\";\"<m><i>\";\"<m><j>\";\"<m><k>\";\"<m><l>\"\n"
	    "mon \"<m><a>\";\"<m><b>\";\"<m><c>\";\"<m><d>\";\"<m><e>\";\"<m><f>\";\"<m><g>\";\"<m><h>\";\"<m><i>\";\"<m><j>\";\"<m><k>\";\"<m><l>\"\n"
	    "d_t_fmt \"<percent-sign><d>\"\nd_fmt \"<percent-sign><d>\"\nt_fmt \"<percent-sign><H>\"\n"
	    "am_pm \"<a>\";\"<p>\"\nt_fmt_ampm \"<percent-sign><I>\"\nEND LC_TIME\n%s",
	    REST);
	ld_ok("cont", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	lc = localeconv();
	pc_check(strcmp(lc->grouping, "\3\2") == 0, "a grouping list continued after the semicolon is not the groups 3 and 2");
	memset(&t, 0, sizeof t);
	t.tm_wday = 1;
	t.tm_mon = 0;
	t.tm_mday = 1;
	t.tm_year = 100;
	pc_check(strftime(buf, sizeof buf, "%a", &t) > 0 && strcmp(buf, "ub") == 0, "%%a for Monday gave \"%s\", not \"ub\"", buf);
	t.tm_wday = 2;
	pc_check(strftime(buf, sizeof buf, "%a", &t) > 0 && strcmp(buf, "uc") == 0, "%%a for Tuesday gave \"%s\", not \"uc\"", buf);
	t.tm_wday = 4;
	pc_check(strftime(buf, sizeof buf, "%a", &t) > 0 && strcmp(buf, "ue") == 0, "%%a for Thursday gave \"%s\", not \"ue\"", buf);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
