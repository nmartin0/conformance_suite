/*
 * The categories of a locale source may appear in any order: a source
 * with them in two different orders makes the same locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.4-locale-grammar.005
 */
#include "pc.h"
#include "ldef.h"
#include <langinfo.h>

#define NUM "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3\nEND LC_NUMERIC\n"
#define MON "LC_MONETARY\nmon_decimal_point \"<semicolon>\"\ncurrency_symbol \"<Q>\"\nEND LC_MONETARY\n"
#define MSG "LC_MESSAGES\nyesexpr \"<circumflex-accent><left-parenthesis><j><right-parenthesis>\"\nnoexpr \"<circumflex-accent><left-parenthesis><k><right-parenthesis>\"\nEND LC_MESSAGES\n"

static void look(const char *which)
{
	const struct lconv *lc = localeconv();

	pc_check(strcmp(lc->decimal_point, ",") == 0, "%s: decimal_point is \"%s\", not \",\"", which, lc->decimal_point);
	pc_check(strcmp(lc->mon_decimal_point, ";") == 0, "%s: mon_decimal_point is \"%s\", not \";\"", which, lc->mon_decimal_point);
	pc_check(strcmp(lc->currency_symbol, "Q") == 0, "%s: currency_symbol is \"%s\", not \"Q\"", which, lc->currency_symbol);
}

int main(void)
{
	char l1[4200], l2[4200];

	pc_begin("XBD.7.4-locale-grammar.005");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("ord1", "cmap", NUM MON MSG LD_CP_CTYPE LD_CP_COLLATE LD_CP_TIME, l1, sizeof l1);
	ld_ok("ord2", "cmap", LD_CP_TIME LD_CP_COLLATE LD_CP_CTYPE MSG MON NUM, l2, sizeof l2);
	pc_test();
	ld_load(l1);
	look("numeric first");
	ld_load(l2);
	look("numeric last");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
