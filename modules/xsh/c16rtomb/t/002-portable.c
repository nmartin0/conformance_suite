/*
 * c32rtomb() and c16rtomb() store the one byte of a portable-character-set
 * member whose Unicode code is given (when the types hold UTF-32 and UTF-16).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.002
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
	pc_begin("XSH.c16rtomb.002");
	pc_notinuse("char16_t and char32_t are not announced as UTF-16 and UTF-32");
#else
	mbstate_t st;
	const char *p;
	char buf[8];
	size_t r;

	pc_begin("XSH.c16rtomb.002");
	if ('A' != 0x41)
		pc_notinuse("the execution character set is not ASCII-based");
	memset(&st, 0, sizeof st);
	pc_test();
	for (p = portable; *p != '\0'; p++) {
		memset(&st, 0, sizeof st);	/* a fresh object per function: the state is not shared between them */
		buf[0] = buf[1] = 'x';
		r = c32rtomb(buf, (char32_t)(unsigned char)*p, &st);
		pc_check(r == 1 && buf[0] == *p, "c32rtomb() of code %d returned %zu and stored %d", *p, r, buf[0]);
		memset(&st, 0, sizeof st);
		buf[0] = buf[1] = 'x';
		r = c16rtomb(buf, (char16_t)(unsigned char)*p, &st);
		pc_check(r == 1 && buf[0] == *p, "c16rtomb() of code %d returned %zu and stored %d", *p, r, buf[0]);
	}
	pc_pass();
#endif
}
