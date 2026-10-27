/*
 * wchar_t codes of the basic character set equal their char codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stddef.h.004
 */
#include <stddef.h>
#include "pc.h"

#define BASIC "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !\"#%&'()*+,-./:;<=>?[\\]^_{|}~\a\b\t\n\v\f\r"
#define WIDE_(x) L ## x
#define WIDE(x) WIDE_(x)

static const char narrow[] = BASIC;
static const wchar_t wide[] = WIDE(BASIC);

int main(void)
{
	pc_begin("XBD.stddef.h.004");
	pc_test();
#ifdef __STDC_MB_MIGHT_NEQ_WC__
	pc_notinuse("__STDC_MB_MIGHT_NEQ_WC__ is defined");
#else
	size_t i;

	pc_check(sizeof narrow == sizeof wide / sizeof wide[0], "the narrow and wide strings differ in length");
	for (i = 0; i < sizeof narrow; i++)
		pc_check((unsigned long)(unsigned char)narrow[i] == (unsigned long)wide[i], "basic character %zu has wide code %lu, narrow code %lu", i, (unsigned long)wide[i], (unsigned long)(unsigned char)narrow[i]);
#endif
	pc_pass();
}
