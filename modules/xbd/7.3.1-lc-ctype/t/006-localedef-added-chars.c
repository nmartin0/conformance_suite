/*
 * A locale built with localedef puts characters added to lower, upper,
 * punct, cntrl and blank in those classes and in the classes they imply
 * (alpha, alnum, graph, print; space for a blank), and in no others; the
 * portable letters, digits and white-space characters are in their classes
 * although the source does not list them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.006
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

#define LOW 0xe1
#define UPP 0xc1
#define PUN 0xa1
#define CTL 0x90
#define BLK 0xa0

static void is_(int b, const char *what, int ok)
{
	pc_check(ok, "byte %#x: %s", b, what);
}

int main(void)
{
	char loc[4200];
	wchar_t wl = 0, wu = 0;
	mbstate_t st;
	char c;

	pc_begin("XBD.7.3.1-lc-ctype.006");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\',
	    "<x-low1> \\xe1\n<x-upp1> \\xc1\n<x-pun1> \\xa1\n<x-ctl1> \\x90\n<x-blk1> \\xa0\n", "", 0);
	ld_ok("added", "cmap",
	    "LC_CTYPE\nlower <x-low1>\nupper <x-upp1>\npunct <x-pun1>\ncntrl <x-ctl1>\nblank <x-blk1>\nEND LC_CTYPE\n"
	    LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	is_(LOW, "not lower", islower(LOW) != 0);
	is_(LOW, "not alpha", isalpha(LOW) != 0);
	is_(LOW, "not alnum", isalnum(LOW) != 0);
	is_(LOW, "not graph", isgraph(LOW) != 0);
	is_(LOW, "not print", isprint(LOW) != 0);
	is_(LOW, "also upper, digit, punct, cntrl or space", !isupper(LOW) && !isdigit(LOW) && !ispunct(LOW) && !iscntrl(LOW) && !isspace(LOW));
	is_(UPP, "not upper", isupper(UPP) != 0);
	is_(UPP, "not alpha", isalpha(UPP) != 0);
	is_(UPP, "not alnum", isalnum(UPP) != 0);
	is_(UPP, "not graph", isgraph(UPP) != 0);
	is_(UPP, "not print", isprint(UPP) != 0);
	is_(UPP, "also lower, digit, punct, cntrl or space", !islower(UPP) && !isdigit(UPP) && !ispunct(UPP) && !iscntrl(UPP) && !isspace(UPP));
	is_(PUN, "not punct", ispunct(PUN) != 0);
	is_(PUN, "not graph", isgraph(PUN) != 0);
	is_(PUN, "not print", isprint(PUN) != 0);
	is_(PUN, "also alpha, digit, space or cntrl", !isalpha(PUN) && !isdigit(PUN) && !isspace(PUN) && !iscntrl(PUN));
	is_(CTL, "not cntrl", iscntrl(CTL) != 0);
	is_(CTL, "also alpha, print, graph, punct or digit", !isalpha(CTL) && !isprint(CTL) && !isgraph(CTL) && !ispunct(CTL) && !isdigit(CTL));
	is_(BLK, "not blank", isblank(BLK) != 0);
	is_(BLK, "not space (blank implies space)", isspace(BLK) != 0);
	is_(BLK, "also alpha, digit, xdigit, upper or lower", !isalpha(BLK) && !isdigit(BLK) && !isxdigit(BLK) && !isupper(BLK) && !islower(BLK));
	c = (char)LOW;
	memset(&st, 0, sizeof st);
	pc_check(mbrtowc(&wl, &c, 1, &st) == 1, "the lowercase byte does not decode");
	c = (char)UPP;
	memset(&st, 0, sizeof st);
	pc_check(mbrtowc(&wu, &c, 1, &st) == 1, "the uppercase byte does not decode");
	pc_check(iswlower((wint_t)wl) && iswalpha((wint_t)wl) && iswprint((wint_t)wl) && !iswupper((wint_t)wl), "the wide code of the added lowercase letter has wrong classes");
	pc_check(iswupper((wint_t)wu) && iswalpha((wint_t)wu) && iswprint((wint_t)wu) && !iswlower((wint_t)wu), "the wide code of the added uppercase letter has wrong classes");
	/* the portable members that are always in their classes */
	pc_check(isupper('A') && isupper('Z') && islower('a') && islower('z') && isdigit('0') && isdigit('9'),
	    "a portable letter or digit is not in its class in the created locale");
	pc_check(isxdigit('a') && isxdigit('F') && isxdigit('7'), "a portable hexadecimal digit is not in xdigit");
	pc_check(isspace(' ') && isspace('\t') && isspace('\n') && isspace('\v') && isspace('\f') && isspace('\r'),
	    "a portable white-space character is not in space");
	pc_check(isblank(' ') && isblank('\t'), "the space or the tab is not in blank");
	pc_check(isprint(' ') != 0, "the space is not in print");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
