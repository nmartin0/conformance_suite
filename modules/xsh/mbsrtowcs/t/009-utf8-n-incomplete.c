/*
 * When the buffer given to mbsnrtowcs() ends inside a character, the
 * conversion stops at the end of the buffer with *src just past the last
 * byte processed, and a later call with the rest completes the character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.009
 */
#include "pc.h"
#include <locale.h>
#include <string.h>
#include <wchar.h>

static void use_utf8(void)
{
	const char *n = pc_find_locale("UTF-8", 1);

	if (n == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, n) == NULL)
		pc_unresolved("setlocale(LC_ALL, \"%s\") failed", n);
}

int main(void)
{
	static const char first[] = "a\xE2\x82", rest[] = "\xAC";
	mbstate_t st;
	const char *src;
	wchar_t whole = 0, w1[4], w2[4];
	size_t n, r1, r2;

	pc_begin("XSH.mbsrtowcs.009");
	use_utf8();
	memset(&st, 0, sizeof st);
	n = mbrtowc(&whole, "\xE2\x82\xAC", 3, &st);
	pc_check(n == 3, "setup: mbrtowc() of a three-byte character returned %zu", n);
	memset(&st, 0, sizeof st);
	w1[0] = w1[1] = w2[0] = w2[1] = 77;
	pc_test();
	src = first;
	r1 = mbsnrtowcs(w1, &src, 3, 4, &st);
	pc_check(r1 == 1, "mbsnrtowcs() returned %zu, expected 1 (only 'a' complete)", r1);
	pc_check(w1[0] == L'a', "mbsnrtowcs() stored the wrong first code");
	pc_check(src == first + 3, "mbsnrtowcs() left *src at the wrong place; expected just past the last byte of the buffer");
	src = rest;
	r2 = mbsnrtowcs(w2, &src, 1, 4, &st);
	pc_check(r2 == 1, "second mbsnrtowcs() returned %zu, expected 1", r2);
	pc_check(w2[0] == whole, "the code completed by the second call differs from the code of one call");
	pc_pass();
}
