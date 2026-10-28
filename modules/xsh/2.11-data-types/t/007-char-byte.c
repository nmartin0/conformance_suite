/*
 * char is a single byte of 8 bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.007
 */
#include "pc.h"
#include <limits.h>

#if CHAR_BIT != 8
#error "CHAR_BIT is not 8"
#endif

int main(void)
{
	unsigned char u = 0xff;
	signed char s = -1;

	pc_begin("XSH.2.11-data-types.007");
	pc_test();
	pc_check(CHAR_BIT == 8, "CHAR_BIT is %d", CHAR_BIT);
	pc_check(sizeof(char) == 1 && sizeof(unsigned char) == 1 && sizeof(signed char) == 1, "a char type is not one byte");
	pc_check(UCHAR_MAX == 255, "UCHAR_MAX is not 255");
	pc_check((unsigned char)(u + 1) == 0 && (unsigned char)s == 255, "an unsigned char does not hold 8 bits");
	pc_pass();
}
