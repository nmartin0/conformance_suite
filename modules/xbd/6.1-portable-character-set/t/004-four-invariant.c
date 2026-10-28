/*
 * The encoded values of period, slash, newline and carriage-return are the
 * same in every locale: the byte that encodes each one is the value of its
 * character constant, and decoding that byte gives the character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.1-portable-character-set.004
 */
#include "pc.h"
#include "pcl.h"
#include <limits.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const char four[] = { '.', '/', '\n', '\r' };
	int i, k;

	pc_begin("XBD.6.1-portable-character-set.004");
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		for (i = 0; i < 4; i++) {
			mbstate_t st;
			char buf[MB_LEN_MAX];
			char in = four[i];
			wchar_t w = 0;
			size_t r;

			memset(&st, 0, sizeof st);
			r = wcrtomb(buf, (wchar_t)in, &st);
			pc_check(r == 1 && buf[0] == in, "locale %s: character %d is not encoded as the byte of its constant",
			    pcl_name[k], i);
			memset(&st, 0, sizeof st);
			r = mbrtowc(&w, &in, 1, &st);
			pc_check(r == 1 && w == (wchar_t)in, "locale %s: the byte of character %d does not decode to it", pcl_name[k], i);
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
