/*
 * mbrtowc() of the null byte returns 0, stores the null wide-character code
 * and leaves the initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.004
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	wchar_t wc = 77;
	size_t r;
	int init;

	pc_begin("XSH.mbrtowc.004");
	memset(&st, 0, sizeof st);
	pc_test();
	r = mbrtowc(&wc, "", 1, &st);
	init = mbsinit(&st);
	pc_check(r == 0, "mbrtowc() of the null byte returned %zu, expected 0", r);
	pc_check(wc == 0, "mbrtowc() of the null byte stored %ld, expected 0", (long)wc);
	pc_check(init != 0, "mbrtowc() of the null byte left a state that is not initial");
	pc_pass();
}
