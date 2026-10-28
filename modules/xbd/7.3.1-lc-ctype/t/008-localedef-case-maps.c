/*
 * Case mappings of a created locale: with toupper given and tolower omitted,
 * only the listed pairs map upward and tolower is the reverse of toupper;
 * with both given each maps only its own pairs; with both omitted the
 * letters a-z and A-Z map to each other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.008
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

#define REST LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char l1[4200], l2[4200], l3[4200];
	int i;

	pc_begin("XBD.7.3.1-lc-ctype.008");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("m1", "cmap", "LC_CTYPE\ntoupper (<a>,<B>)\nEND LC_CTYPE\n" REST, l1, sizeof l1);
	ld_ok("m2", "cmap", "LC_CTYPE\ntoupper (<a>,<A>)\ntolower (<B>,<b>)\nEND LC_CTYPE\n" REST, l2, sizeof l2);
	ld_ok("m3", "cmap", "LC_CTYPE\nupper <A>\nEND LC_CTYPE\n" REST, l3, sizeof l3);
	pc_test();
	ld_load(l1);
	pc_check(toupper('a') == 'B', "toupper('a') is not 'B' with the pair (a,B)");
	pc_check(toupper('b') == 'b', "toupper('b') changed although only (a,B) was given");
	pc_check(tolower('B') == 'a', "tolower('B') is not 'a' although tolower was omitted (reverse of toupper)");
	pc_check(tolower('A') == 'A', "tolower('A') changed although the reverse of toupper has only B");
	pc_check(towupper(L'a') == L'B' && towupper(L'b') == L'b', "towupper does not follow toupper");
	pc_check(towlower(L'B') == L'a' && towlower(L'A') == L'A', "towlower does not follow the reverse of toupper");
	ld_load(l2);
	pc_check(toupper('a') == 'A' && toupper('b') == 'b', "toupper in the locale with both keywords is wrong");
	pc_check(tolower('B') == 'b' && tolower('A') == 'A', "tolower in the locale with both keywords is wrong");
	pc_check(towupper(L'a') == L'A' && towlower(L'B') == L'b' && towupper(L'b') == L'b' && towlower(L'A') == L'A',
	    "the wide mappings in the locale with both keywords are wrong");
	ld_load(l3);
	for (i = 0; i < 26; i++) {
		pc_check(toupper((unsigned char)pcc_lower[i]) == (unsigned char)pcc_upper[i], "toupper of letter #%d is wrong", i);
		pc_check(tolower((unsigned char)pcc_upper[i]) == (unsigned char)pcc_lower[i], "tolower of letter #%d is wrong", i);
		pc_check(towupper((wint_t)pcc_lower[i]) == (wint_t)pcc_upper[i], "towupper of letter #%d is wrong", i);
		pc_check(towlower((wint_t)pcc_upper[i]) == (wint_t)pcc_lower[i], "towlower of letter #%d is wrong", i);
	}
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
