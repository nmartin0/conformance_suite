/*
 * mbrtoc32() and mbrtoc16() give a portable-character-set member its
 * Unicode code (when the types hold UTF-32 and UTF-16).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.002
 */
#include "pc.h"
#include <string.h>
#include <uchar.h>
#include <wchar.h>

/* every member of the portable character set that has a graphic or control form */
static const char portable[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz{|}~\a\b\t\n\v\f\r";

int main(void)
{
#if !defined(__STDC_UTF_32__) || !defined(__STDC_UTF_16__)
	pc_begin("XSH.mbrtoc16.002");
	pc_notinuse("char16_t and char32_t are not announced as UTF-16 and UTF-32");
#else
	mbstate_t st32, st16;	/* one per function: a state is not promised to carry from one to the other */
	const char *p;
	char32_t c32;
	char16_t c16;
	size_t r;

	pc_begin("XSH.mbrtoc16.002");
	if ('A' != 0x41)
		pc_notinuse("the execution character set is not ASCII-based");
	memset(&st32, 0, sizeof st32);
	memset(&st16, 0, sizeof st16);
	pc_test();
	for (p = portable; *p != '\0'; p++) {
		c32 = 77;
		r = mbrtoc32(&c32, p, 1, &st32);
		pc_check(r == 1 && c32 == (char32_t)(unsigned char)*p, "mbrtoc32() of character %d returned %zu and stored %ld", *p, r, (long)c32);
		c16 = 77;
		r = mbrtoc16(&c16, p, 1, &st16);
		pc_check(r == 1 && c16 == (char16_t)(unsigned char)*p, "mbrtoc16() of character %d returned %zu and stored %ld", *p, r, (long)c16);
	}
	pc_pass();
#endif
}
