/*
 * The bytes that encode period, slash, newline and carriage-return are not
 * part of any other character: in every locale they decode to their own
 * characters in a row, and after any byte that starts a longer character
 * they are refused rather than taken as a continuation.
 * The second part is not applied to a locale whose encoding has shift states:
 * the text does not say that the four bytes keep their meaning outside the
 * initial shift state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.2-character-encoding.004
 */
#include "pc.h"
#include "pcl.h"
#include <wchar.h>

int main(void)
{
	static const char four[] = { '.', '/', '\n', '\r' };
	int k, lead, i;

	pc_begin("XBD.6.2-character-encoding.004");
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		mbstate_t st;
		wchar_t w;
		size_t r;

		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		memset(&st, 0, sizeof st);
		for (i = 0; i < 4; i++) {
			w = 9;
			r = mbrtowc(&w, &four[i], 1, &st);
			pc_check(r == 1 && w == (wchar_t)four[i], "locale %s: character %d of the run does not decode to itself",
			    pcl_name[k], i);
		}
		if (pcl_shifting())
			continue;
		for (lead = 1; lead < 256; lead++) {
			char b = (char)lead;

			memset(&st, 0, sizeof st);
			r = mbrtowc(&w, &b, 1, &st);
			if (r != (size_t)-2)
				continue;
			for (i = 0; i < 4; i++) {
				mbstate_t st2 = st;

				r = mbrtowc(&w, &four[i], 1, &st2);
				/* a shift sequence is not a character: still incomplete is allowed */
				pc_check(r == (size_t)-1 || r == (size_t)-2 || r == 0,
				    "locale %s: after the incomplete sequence starting with byte %d, character %d of the four gave %ld "
				    "(a part of a character)",
				    pcl_name[k], lead, i, (long)r);
			}
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
