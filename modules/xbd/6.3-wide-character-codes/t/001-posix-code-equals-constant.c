/*
 * In the POSIX locale the wide-character code of each member of the
 * portable character set equals the value of its character constant:
 * decoding the member's byte gives that value (btowc and mbrtowc), and
 * encoding that value gives the member's byte (wcrtomb and wctob).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.3-wide-character-codes.001
 */
#include "pc.h"
#include "pcchars.h"
#include <limits.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	int i;

	pc_begin("XBD.6.3-wide-character-codes.001");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (i = 0; i < PCC_N; i++) {
		char ch = pcc_all[i];
		char out[MB_LEN_MAX];
		mbstate_t st;
		wchar_t w = 3;
		size_t r;

		pc_check(btowc((unsigned char)ch) == (wint_t)ch, "member %d: btowc of its byte is not the value of its constant", i);
		memset(&st, 0, sizeof st);
		r = mbrtowc(&w, &ch, 1, &st);
		pc_check(r == (ch == '\0' ? 0u : 1u) && w == (wchar_t)ch, "member %d: mbrtowc does not give the value of its constant", i);
		memset(&st, 0, sizeof st);
		r = wcrtomb(out, (wchar_t)ch, &st);
		pc_check(r == 1 && out[0] == ch, "member %d: wcrtomb of the value of its constant is not its byte", i);
		pc_check(wctob((wint_t)ch) == (int)(unsigned char)ch, "member %d: wctob of the value of its constant is not its byte", i);
	}
	pc_pass();
}
