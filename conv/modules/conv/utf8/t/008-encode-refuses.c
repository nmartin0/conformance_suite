/*
 * wcrtomb refuses the surrogate range and values above U+10FFFF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.008
 * MUTANT wcrtomb.surrogate
 * MUTANT wcrtomb.above
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/u8.h"

static void refuse(unsigned long v)
{
	unsigned char got[16];
	mbstate_t ps;
	size_t r;

	memset(&ps, 0, sizeof ps);
	memset(got, 0xAA, sizeof got);
	errno = 0;
	r = wcrtomb((char *)got, (wchar_t)v, &ps);
	pc_check(r == (size_t)-1, "wcrtomb(%lX) returned %zd instead of failing", v, (ssize_t)r);
}

int main(void)
{
	unsigned long v;

	pc_begin("CONV.utf8.008");
#ifndef __STDC_ISO_10646__
	pc_notinuse("wide characters are not documented as Unicode code points (__STDC_ISO_10646__ is not defined)");
#endif
	u8_locale();
	pc_test();
	for (v = 0xD800; v <= 0xDFFF; v++)
		refuse(v);
	refuse(0x110000);
	refuse(0x1FFFFF);
	refuse(0x200000);
	refuse(0x3FFFFFF);
	refuse(0x7FFFFFFF);
	pc_pass();
}
