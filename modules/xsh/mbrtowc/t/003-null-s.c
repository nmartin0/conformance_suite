/*
 * mbrtowc() with a null s acts as a conversion of the null byte: it
 * returns 0, stores nothing and leaves the initial state, whatever pwc and
 * n are.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.003
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	wchar_t wc = 77;
	size_t r1, r2;
	int i1, i2;

	pc_begin("XSH.mbrtowc.003");
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = mbrtowc(&wc, NULL, 5, &st);
	i1 = mbsinit(&st);
	r2 = mbrtowc(&wc, NULL, 0, &st);
	i2 = mbsinit(&st);
	pc_check(r1 == 0, "mbrtowc(&wc, NULL, 5, &st) returned %zu, expected 0", r1);
	pc_check(r2 == 0, "mbrtowc(&wc, NULL, 0, &st) returned %zu, expected 0", r2);
	pc_check(wc == 77, "mbrtowc() with a null s stored into *pwc");
	pc_check(i1 != 0 && i2 != 0, "mbrtowc() with a null s left a state that is not initial");
	pc_pass();
}
