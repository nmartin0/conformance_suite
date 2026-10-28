/*
 * In the POSIX locale cntrl holds at least the null character, the seven
 * portable controls and twenty-five more (33 or more bytes in all) and
 * none of them is alpha or print; punct holds the 32 portable punctuation
 * characters and nothing from space, alpha, digit or cntrl; graph is
 * exactly alpha, digit and punct together; print is graph plus the space.
 * Checked over all 256 bytes, through the byte and the wide functions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.003
 */
#include "pc.h"
#include "pcchars.h"
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

int main(void)
{
	int b, ncntrl = 0;

	pc_begin("XBD.7.3.1-lc-ctype.003");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (b = 0; b < 256; b++) {
		unsigned f = pcc_flags[b];
		wint_t w = btowc(b);
		int cn = iscntrl(b) != 0, pu = ispunct(b) != 0, gr = isgraph(b) != 0, pr = isprint(b) != 0;
		int al = isalpha(b) != 0, di = isdigit(b) != 0, sp = isspace(b) != 0;

		pc_check(w != WEOF, "byte %d is not a character of the POSIX locale", b);
		if (f & PCC_CNTRL)
			pc_check(cn, "byte %d (a portable control character) is not in cntrl", b);
		if (f & PCC_PUNCT)
			pc_check(pu, "byte %d (a portable punctuation character) is not in punct", b);
		if (cn) {
			ncntrl++;
			pc_check(!al && !pr && !gr, "byte %d is in cntrl and also in alpha, graph or print", b);
		}
		if (pu)
			pc_check(b != (unsigned char)' ' && !al && !di && !cn && !sp, "byte %d is in punct and also in space, alpha, digit or cntrl", b);
		pc_check(gr == (al || di || pu), "byte %d: graph is %d but alpha|digit|punct is %d", b, gr, al || di || pu);
		pc_check(pr == (gr || b == (unsigned char)' '), "byte %d: print is %d but graph|space is %d", b, pr,
		    gr || b == (unsigned char)' ');
		pc_check((iswcntrl(w) != 0) == cn && (iswpunct(w) != 0) == pu && (iswgraph(w) != 0) == gr && (iswprint(w) != 0) == pr,
		    "byte %d: the wide functions disagree with the byte functions on cntrl, punct, graph or print", b);
		pc_check((iswctype(w, wctype("cntrl")) != 0) == cn && (iswctype(w, wctype("punct")) != 0) == pu &&
		    (iswctype(w, wctype("graph")) != 0) == gr && (iswctype(w, wctype("print")) != 0) == pr,
		    "byte %d: iswctype() disagrees with the byte functions on cntrl, punct, graph or print", b);
	}
	pc_check(ncntrl >= 33, "only %d bytes are in cntrl; the null character, the portable controls and 25 more are required", ncntrl);
	pc_pass();
}
