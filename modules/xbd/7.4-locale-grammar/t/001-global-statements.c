/*
 * The global statements comment_char and escape_char change the comment
 * and escape characters of the source: a line starting with the new
 * comment character is ignored (even if it would end the category), and
 * the new escape character introduces character encodings. Without the
 * statements the number sign starts comments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.4-locale-grammar.001
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc1[4200], loc2[4200], text[3000];
	const struct lconv *lc;

	pc_begin("XBD.7.4-locale-grammar.001");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	(void)snprintf(text, sizeof text,
	    "escape_char /\ncomment_char %%\n%% END LC_NUMERIC would end the category\nLC_NUMERIC\n"
	    "%% decimal_point \"<period>\"\ndecimal_point \"/x%02X\"\nthousands_sep \"<period>\"\ngrouping 3\nEND LC_NUMERIC\n%s",
	    (unsigned)(unsigned char)',', REST);
	ld_ok("glob", "cmap", text, loc1, sizeof loc1);
	(void)snprintf(text, sizeof text,
	    "# END LC_NUMERIC would end the category\nLC_NUMERIC\n# decimal_point \"<period>\"\ndecimal_point \"<comma>\"\n"
	    "thousands_sep \"<period>\"\ngrouping 3\nEND LC_NUMERIC\n%s",
	    REST);
	ld_ok("defc", "cmap", text, loc2, sizeof loc2);
	pc_test();
	ld_load(loc1);
	lc = localeconv();
	pc_check(strcmp(lc->decimal_point, ",") == 0, "with comment_char %% and escape_char /, decimal_point is \"%s\", not \",\"", lc->decimal_point);
	pc_check(strcmp(lc->thousands_sep, ".") == 0, "with comment_char %% and escape_char /, thousands_sep is \"%s\", not \".\"", lc->thousands_sep);
	ld_load(loc2);
	lc = localeconv();
	pc_check(strcmp(lc->decimal_point, ",") == 0, "with the default comment character, decimal_point is \"%s\", not \",\"", lc->decimal_point);
	pc_check(strcmp(lc->thousands_sep, ".") == 0, "with the default comment character, thousands_sep is \"%s\", not \".\"", lc->thousands_sep);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
