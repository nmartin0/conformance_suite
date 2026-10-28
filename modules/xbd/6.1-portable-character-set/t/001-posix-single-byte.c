/*
 * Each member of the portable character set is a complete single-byte
 * character in the POSIX locale; the members have distinct encodings, the
 * null character is the zero byte and every other member is positive when
 * held in a char.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.1-portable-character-set.001
 */
#include "pc.h"
#include "pcchars.h"
#include <locale.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	unsigned char seen[256];
	int i;

	pc_begin("XBD.6.1-portable-character-set.001");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	memset(seen, 0, sizeof seen);
	pc_test();
	for (i = 0; i < PCC_N; i++) {
		char ch = pcc_all[i];
		unsigned char u = (unsigned char)ch;
		mbstate_t st;
		wchar_t w = 1;
		size_t r;

		memset(&st, 0, sizeof st);
		r = mbrtowc(&w, &ch, 1, &st);
		if (u == 0) {
			pc_check(r == 0, "member %d (the null character): mbrtowc gave %lu, not 0", i, (unsigned long)r);
		} else {
			pc_check(r == 1, "member %d (byte %u): mbrtowc gave %lu, not 1 (one byte, one character)", i,
			    (unsigned)u, (unsigned long)r);
			pc_check(ch > 0, "member %d (byte %u) is not positive in a char", i, (unsigned)u);
		}
		pc_check(mbsinit(&st) != 0, "member %d: the conversion state is not initial afterwards", i);
		pc_check(seen[u] == 0, "two members of the set are encoded as byte %u", (unsigned)u);
		seen[u] = 1;
	}
	pc_check(pcc_all[0] == '\0', "the null character is not the zero byte");
	pc_pass();
}
