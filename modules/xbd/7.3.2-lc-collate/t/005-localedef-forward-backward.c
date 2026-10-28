/*
 * With two weights per character, the second weight is compared from the
 * start of the strings under "forward" and from the end under "backward",
 * and only when the first weights leave the strings equal; a and A share
 * their first weight.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.005
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES
#define ORDER "<a> <a>;<a>\n<A> <a>;<A>\n<b> <b>;<b>\n<B> <b>;<B>\nUNDEFINED IGNORE;...\norder_end\nEND LC_COLLATE\n"

int main(void)
{
	char lf[4200], lb[4200];

	pc_begin("XBD.7.3.2-lc-collate.005");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("fwd", "cmap", "LC_COLLATE\norder_start forward;forward\n" ORDER REST, lf, sizeof lf);
	ld_ok("bwd", "cmap", "LC_COLLATE\norder_start forward;backward\n" ORDER REST, lb, sizeof lb);
	pc_test();
	ld_load(lf);
	pc_check(strcoll("a", "A") < 0 && strcoll("A", "a") > 0, "forward: a does not collate before A on the second weight");
	pc_check(strcoll("A", "b") < 0 && strcoll("B", "a") > 0, "forward: the first weight does not decide before the second");
	pc_check(strcoll("Ab", "aB") > 0, "forward: Ab does not collate after aB (the second weights are compared from the start: A after a)");
	pc_check(strcoll("aB", "Ab") < 0 && strcoll("ab", "Ab") < 0, "forward: aB or ab does not collate before Ab");
	ld_load(lb);
	pc_check(strcoll("a", "A") < 0 && strcoll("A", "a") > 0, "backward: a does not collate before A on the second weight");
	pc_check(strcoll("A", "b") < 0 && strcoll("B", "a") > 0, "backward: the first weight does not decide before the second");
	pc_check(strcoll("Ab", "aB") < 0, "backward: Ab does not collate before aB (the second weights are compared from the end: b before B)");
	pc_check(strcoll("aB", "Ab") > 0, "backward: aB does not collate after Ab");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
