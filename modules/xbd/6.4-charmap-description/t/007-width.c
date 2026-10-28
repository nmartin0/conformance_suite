/*
 * The WIDTH and WIDTH_DEFAULT declarations after the mapping lines set
 * column widths: a printable character listed with width 2 (singly or in
 * a range of names) has width 2 in wcwidth(), a printable character not
 * listed has the default, and without WIDTH_DEFAULT the default is 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.007
 */
#include "pc.h"
#include "ldef.h"
#include <wchar.h>

#define REST LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

static wchar_t wc_of(int byte)
{
	char c = (char)byte;
	mbstate_t st;
	wchar_t w = 0;

	memset(&st, 0, sizeof st);
	if (mbrtowc(&w, &c, 1, &st) != 1)
		pc_fail("the byte %#x does not decode", byte);
	return w;
}

int main(void)
{
	char loc[4200], loc1[4200];
	int i;

	pc_begin("XBD.6.4-charmap-description.007");
	ld_need();
	pcc_init();
	for (i = 0; i < PCC_N; i++)
		if ((unsigned char)pcc_all[i] >= 0xe1 && (unsigned char)pcc_all[i] <= 0xe8)
			pc_notinuse("a portable character is encoded in the high range the test uses");
	ld_charmap("cmap", LD_HDR, '\\', "<x-low1> \\xe1\n<x-low2> \\xe2\n<r01>...<r03> \\xe5\n",
	    "WIDTH_DEFAULT 3\nWIDTH\n<x-low1> 2\n<r01>...<r03> 2\nEND WIDTH\n", 0);
	ld_ok("width", "cmap",
	    "LC_CTYPE\nlower <x-low1>;<x-low2>;<r01>;<r02>;<r03>\nEND LC_CTYPE\n" REST, loc, sizeof loc);
	ld_charmap("cmap1", LD_HDR, '\\', "<x-low1> \\xe1\n", "", 0);
	ld_ok("width1", "cmap1", "LC_CTYPE\nlower <x-low1>\nEND LC_CTYPE\n" REST, loc1, sizeof loc1);
	pc_test();
	ld_load(loc);
	pc_check(wcwidth(wc_of(0xe1)) == 2, "wcwidth of the character listed with WIDTH 2 is %d, not 2", wcwidth(wc_of(0xe1)));
	pc_check(wcwidth(wc_of(0xe5)) == 2 && wcwidth(wc_of(0xe6)) == 2 && wcwidth(wc_of(0xe7)) == 2,
	    "wcwidth of the characters listed with WIDTH 2 as a range is not 2");
	pc_check(wcwidth(wc_of(0xe2)) == 3, "wcwidth of a character not listed under WIDTH is %d, not the default 3", wcwidth(wc_of(0xe2)));
	pc_check(wcwidth(L'a') == 3, "wcwidth of the portable letter a is %d, not the default 3", wcwidth(L'a'));
	ld_load(loc1);
	pc_check(wcwidth(wc_of(0xe1)) == 1, "wcwidth without WIDTH declarations is %d, not 1", wcwidth(wc_of(0xe1)));
	pc_check(wcwidth(L'a') == 1, "wcwidth of the portable letter a without WIDTH declarations is %d, not 1", wcwidth(L'a'));
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
