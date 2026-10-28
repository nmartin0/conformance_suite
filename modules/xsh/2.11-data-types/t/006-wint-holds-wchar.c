/*
 * wint_t holds the wide characters of the POSIX locale and WEOF, which is none of them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.006
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

int main(void)
{
	int i, mapped = 0;

	pc_begin("XSH.2.11-data-types.006");
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(\"C\") failed");
	pc_test();
	for (i = 0; i < 256; i++) {
		wint_t w = btowc(i);
		wchar_t wc;
		wint_t back;

		if (w == WEOF)
			continue;
		mapped++;
		wc = (wchar_t)w;
		back = (wint_t)wc;
		pc_check(back == w, "the code %d maps to %lu, which does not survive wchar_t and back (%lu)", i,
		    (unsigned long)w, (unsigned long)back);
		pc_check(w != WEOF, "the code %d maps to WEOF's value", i);
	}
	pc_check(mapped >= 128, "only %d of the codes 0 to 255 map to a wide character", mapped);
	pc_check(sizeof(wint_t) >= sizeof(wchar_t) || mapped >= 128, "wint_t is narrower than wchar_t");
	pc_pass();
}
