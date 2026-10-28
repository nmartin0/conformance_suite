/*
 * With the position directive on the only weight level, elements subject
 * to IGNORE are not removed: the string whose first non-ignored element
 * comes after fewer ignored elements collates first, so ab collates before
 * a-b and a-b before -ab, with the hyphen ignored; without the directive
 * the three collate as equal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.009
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES
#define ORDER "<hyphen-minus> IGNORE\n<a>\n<b>\nUNDEFINED\norder_end\nEND LC_COLLATE\n"

int main(void)
{
	char lp[4200], ln[4200];

	pc_begin("XBD.7.3.2-lc-collate.009");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("pos", "cmap", "LC_COLLATE\norder_start forward,position\n" ORDER REST, lp, sizeof lp);
	ld_ok("nopos", "cmap", "LC_COLLATE\norder_start forward\n" ORDER REST, ln, sizeof ln);
	pc_test();
	ld_load(ln);
	pc_check(strcoll("ab", "a-b") == 0 && strcoll("a-b", "-ab") == 0, "without position, ab, a-b and -ab do not collate as equal");
	ld_load(lp);
	pc_check(strcoll("ab", "a-b") < 0, "with position, ab does not collate before a-b");
	pc_check(strcoll("a-b", "-ab") < 0, "with position, a-b does not collate before -ab");
	pc_check(strcoll("ab", "-ab") < 0, "with position, ab does not collate before -ab");
	pc_check(strcoll("ab", "ab") == 0, "with position, ab and ab do not collate as equal");
	pc_check(strcoll("ab", "ba") < 0 && strcoll("a-a", "a-b") < 0,
	    "with position, elements in the same relative position are not ordered by their collating values");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
