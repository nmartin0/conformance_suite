/*
 * The POSIX locale has 256 single-byte characters: MB_CUR_MAX is 1, every
 * byte value decodes to a character (the zero byte to the null character),
 * the 256 characters have distinct codes and each encodes back to its byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.2-character-encoding.001
 */
#include "pc.h"
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	wchar_t code[256];
	int b, c;

	pc_begin("XBD.6.2-character-encoding.001");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	pc_check(MB_CUR_MAX == 1, "MB_CUR_MAX is %lu in the POSIX locale", (unsigned long)MB_CUR_MAX);
	for (b = 0; b < 256; b++) {
		char ch = (char)b;
		char out[MB_LEN_MAX];
		mbstate_t st;
		size_t r;

		memset(&st, 0, sizeof st);
		code[b] = 0x7fff;
		r = mbrtowc(&code[b], &ch, 1, &st);
		pc_check(r == (b == 0 ? 0u : 1u), "byte %d: mbrtowc gave %lu", b, (unsigned long)r);
		for (c = 0; c < b; c++)
			pc_check(code[c] != code[b], "bytes %d and %d decode to the same code", c, b);
		memset(&st, 0, sizeof st);
		r = wcrtomb(out, code[b], &st);
		pc_check(r == 1 && out[0] == ch, "byte %d: wcrtomb of its code gave %lu bytes, first %u", b, (unsigned long)r,
		    (unsigned)(unsigned char)out[0]);
	}
	pc_pass();
}
