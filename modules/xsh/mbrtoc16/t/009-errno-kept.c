/*
 * A successful mbrtoc32() or mbrtoc16() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.009
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st32, st16;	/* one per function */
	char32_t c32;
	char16_t c16;
	size_t r[4];
	int e[4], i;

	pc_begin("XSH.mbrtoc16.009");
	memset(&st32, 0, sizeof st32);
	memset(&st16, 0, sizeof st16);
	pc_test();
	errno = EDOM;
	r[0] = mbrtoc32(&c32, "A", 1, &st32);
	e[0] = errno;
	errno = EDOM;
	r[1] = mbrtoc32(&c32, "", 1, &st32);
	e[1] = errno;
	errno = EDOM;
	r[2] = mbrtoc16(&c16, "A", 1, &st16);
	e[2] = errno;
	errno = EDOM;
	r[3] = mbrtoc16(&c16, "", 1, &st16);
	e[3] = errno;
	pc_check(r[0] == 1 && r[1] == 0 && r[2] == 1 && r[3] == 0, "the conversions returned %zu, %zu, %zu, %zu, expected 1, 0, 1, 0", r[0], r[1], r[2], r[3]);
	for (i = 0; i < 4; i++)
		pc_check(e[i] == EDOM, "conversion %d changed errno from EDOM to %s", i, pc_errname(e[i]));
	pc_pass();
}
