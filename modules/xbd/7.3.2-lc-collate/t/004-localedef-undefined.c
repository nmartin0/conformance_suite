/*
 * A collation order started with order_start and ended with UNDEFINED puts
 * the listed characters first in the listed order and all others after
 * them in ascending order of their encoded values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.004
 */
#include "pc.h"
#include "ldef.h"

static int sg(int v)
{
	return v < 0 ? -1 : v > 0;
}

/* sign of the difference of the encoded values of two portable characters */
static int ev(char a, char b)
{
	return (unsigned char)a < (unsigned char)b ? -1 : (unsigned char)a > (unsigned char)b;
}

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];

	pc_begin("XBD.7.3.2-lc-collate.004");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("undef", "cmap",
	    "LC_COLLATE\norder_start forward\n<b>\n<a>\n<c>\nUNDEFINED\norder_end\nEND LC_COLLATE\n" REST, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(strcoll("b", "a") < 0, "b does not collate before a although it is listed first");
	pc_check(strcoll("a", "c") < 0, "a does not collate before c");
	pc_check(strcoll("c", "d") < 0, "the listed c does not collate before the unlisted d");
	pc_check(strcoll("c", "A") < 0, "the listed c does not collate before the unlisted A");
	pc_check(strcoll("b", "B") < 0, "the listed b does not collate before the unlisted B");
	pc_check(strcoll("A", "B") < 0 && strcoll("d", "e") < 0, "the unlisted characters are not in ascending order of their encoded values");
	pc_check(sg(strcoll("B", "d")) == ev('B', 'd') && sg(strcoll("0", "A")) == ev('0', 'A') && sg(strcoll("d", "A")) == ev('d', 'A'),
	    "unlisted characters of different kinds are not in the order of their encoded values");
	pc_check(strcoll("ba", "ab") < 0, "ba does not collate before ab");
	pc_check(strcoll("ab", "ac") < 0 && strcoll("ca", "cb") > 0, "strings are not compared character by character in the listed order");
	pc_check(strcoll("a", "a") == 0 && strcoll("", "b") < 0, "equal strings or the empty string collate wrongly");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
