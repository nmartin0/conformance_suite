/*
 * A charmap with <mb_cur_max> 2 and two-byte characters written as two
 * concatenated constants makes a locale in which MB_CUR_MAX is 2, the
 * two bytes decode as one character in sequence order, the first byte
 * alone is an incomplete character, and the character is in the class the
 * source gives it. A locale built from a charmap without the declaration
 * has MB_CUR_MAX 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.006
 */
#include "pc.h"
#include "ldef.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#define REST LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200], loc1[4200];
	wchar_t w1, w2, w3, ws[10];
	mbstate_t st;
	size_t r;
	int i;

	pc_begin("XBD.6.4-charmap-description.006");
	ld_need();
	pcc_init();
	for (i = 0; i < PCC_N; i++)
		if ((unsigned char)pcc_all[i] >= 0xe1 && (unsigned char)pcc_all[i] <= 0xe3)
			pc_notinuse("a portable character is encoded in the high range the test uses");
	ld_charmap("cmap2", "<code_set_name> PCTEST2\n<mb_cur_max> 2\n<mb_cur_min> 1\n", '\\',
	    "<m-one> \\xe1\\xe2\n<m-two> \\xe1\\xe3\n", "", 0);
	ld_ok("mb2", "cmap2",
	    "LC_CTYPE\nlower <m-one>;<m-two>\nEND LC_CTYPE\n" REST, loc, sizeof loc);
	ld_charmap("cmap1", LD_HDR, '\\', "", "", 0);
	ld_ok("mb1", "cmap1", LD_CP_CTYPE REST, loc1, sizeof loc1);
	pc_test();
	ld_load(loc);
	pc_check(MB_CUR_MAX == 2, "MB_CUR_MAX is %lu, not 2", (unsigned long)MB_CUR_MAX);
	memset(&st, 0, sizeof st);
	w1 = w2 = 0;
	r = mbrtowc(&w1, "\xe1\xe2", 2, &st);
	pc_check(r == 2, "mbrtowc of the two bytes e1 e2 returned %ld, not 2", (long)r);
	memset(&st, 0, sizeof st);
	r = mbrtowc(&w2, "\xe1\xe3", 2, &st);
	pc_check(r == 2, "mbrtowc of the two bytes e1 e3 returned %ld, not 2", (long)r);
	pc_check(w1 != w2 && w1 != 0 && w2 != 0, "the two characters have equal or zero wide codes");
	memset(&st, 0, sizeof st);
	w3 = 0;
	r = mbrtowc(&w3, "\xe1", 1, &st);
	pc_check(r == (size_t)-2, "mbrtowc of the lone first byte returned %ld, not the incomplete-character result", (long)r);
	r = mbrtowc(&w3, "\xe2", 1, &st);
	pc_check(r == 1, "mbrtowc of the second byte after the first returned %ld, not 1", (long)r);
	pc_check(w3 == w1, "the character built from two calls differs from the one decoded at once");
	pc_check(iswlower((wint_t)w1) && iswlower((wint_t)w2), "a two-byte character listed as lower-case is not lower-case");
	r = mbstowcs(ws, "a\xe1\xe2" "b", 10);
	pc_check(r == 3 && ws[0] == L'a' && ws[1] == w1 && ws[2] == L'b', "mbstowcs of a, a two-byte character and b gave %ld wide characters", (long)r);
	ld_load(loc1);
	pc_check(MB_CUR_MAX == 1, "MB_CUR_MAX is %lu in a locale from a charmap with the default of one byte", (unsigned long)MB_CUR_MAX);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
