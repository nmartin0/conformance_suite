/*
 * mbrtowc() accepts a null pwc (nothing stored) and a null ps (its own
 * state).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.002
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	wchar_t wc = 77;
	size_t r1, r2;

	pc_begin("XSH.mbrtowc.002");
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = mbrtowc(NULL, "A", 1, &st);
	r2 = mbrtowc(&wc, "A", 1, NULL);
	pc_check(r1 == 1, "mbrtowc() with a null pwc returned %zu, expected 1", r1);
	pc_check(r2 == 1, "mbrtowc() with a null ps returned %zu, expected 1", r2);
	pc_check(wc == (wchar_t)'A', "mbrtowc() with a null ps stored %ld, expected the value of 'A'", (long)wc);
	pc_pass();
}
