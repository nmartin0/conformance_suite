/*
 * In a UTF-8 locale mbsrtowcs() counts characters, stops after len of them
 * with *src just past the last whole character, and can be resumed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.006
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
	static const char s[] = "a\xC3\xA9\xE2\x82\xAC";
	mbstate_t st;
	const char *src;
	wchar_t w[8];
	size_t r1, r2, r3;

	pc_begin("XSH.mbsrtowcs.006");
	use_utf8();
	pc_test();
	memset(&st, 0, sizeof st);
	src = s;
	r1 = mbsrtowcs(w, &src, 8, &st);
	pc_check(r1 == 3, "mbsrtowcs() returned %zu, expected 3", r1);
	pc_check(src == NULL, "mbsrtowcs() left *src non-null after reaching the null byte");
	memset(&st, 0, sizeof st);
	src = s;
	r2 = mbsrtowcs(w, &src, 2, &st);
	pc_check(r2 == 2, "mbsrtowcs() with len 2 returned %zu, expected 2", r2);
	pc_check(src == s + 3, "mbsrtowcs() left *src at offset %ld, expected 3", src == NULL ? -1L : (long)(src - s));
	r3 = mbsrtowcs(w + 4, &src, 4, &st);
	pc_check(r3 == 1 && src == NULL, "resumed mbsrtowcs() returned %zu, expected 1 with *src null", r3);
	pc_pass();
}
