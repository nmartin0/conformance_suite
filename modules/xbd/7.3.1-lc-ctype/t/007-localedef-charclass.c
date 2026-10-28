/*
 * A locale-specific class declared with charclass and filled in the source
 * can be used through wctype() and iswctype(): its members are in it and
 * other characters are not; a class that is declared but given no members
 * exists and is empty; a name that no class has gives no handle.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.007
 */
#include "pc.h"
#include "ldef.h"
#include <wchar.h>
#include <wctype.h>

int main(void)
{
	static const char vow[] = "aeiou";
	char loc[4200];
	wctype_t hv, he;
	int i;

	pc_begin("XBD.7.3.1-lc-ctype.007");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("class", "cmap",
	    "LC_CTYPE\ncharclass pcvow;pcnone\npcvow <a>;<e>;<i>;<o>;<u>\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY
	        LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	hv = wctype("pcvow");
	he = wctype("pcnone");
	pc_check(hv != 0, "wctype(\"pcvow\") gave no handle for a declared class");
	pc_check(he != 0, "wctype(\"pcnone\") gave no handle for a class that was declared and left empty");
	pc_check(wctype("pcabsent") == 0, "wctype of a name no class has gave a handle");
	for (i = 0; i < 26; i++) {
		wint_t w = (wint_t)pcc_lower[i];
		int member = strchr(vow, pcc_lower[i]) != NULL;

		pc_check((iswctype(w, hv) != 0) == member, "letter #%d is %s pcvow", i, member ? "missing from" : "in");
		pc_check(iswctype(w, he) == 0, "letter #%d is in the empty class", i);
		pc_check(iswctype((wint_t)pcc_upper[i], hv) == 0, "uppercase letter #%d is in pcvow", i);
	}
	pc_check(iswlower((wint_t)'e') != 0 && iswalpha((wint_t)'e') != 0, "defining a class changed the standard classes");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
