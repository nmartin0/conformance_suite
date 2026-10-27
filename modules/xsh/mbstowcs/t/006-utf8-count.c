/*
 * In a UTF-8 locale mbstowcs() counts characters, not bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.006
 */
#include "pc.h"
#include <locale.h>
#include <stdlib.h>

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
	wchar_t w[8];
	size_t r1, r2;

	pc_begin("XSH.mbstowcs.006");
	use_utf8();
	pc_test();
	r1 = mbstowcs(w, s, 8);
	pc_check(r1 == 3, "mbstowcs() returned %zu, expected 3", r1);
	pc_check(w[0] == L'a', "mbstowcs() stored %ld for the first character", (long)w[0]);
	r2 = mbstowcs(w, s, 2);
	pc_check(r2 == 2, "mbstowcs() with n 2 returned %zu, expected 2", r2);
	pc_pass();
}
