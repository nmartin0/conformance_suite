/*
 * mbstowcs() does not look at bytes after the null byte, even invalid ones.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.008
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
	static const char s[] = { 'a', 'b', '\0', '\x80', '\xC3', '\0' };
	wchar_t w[8];
	size_t r;

	pc_begin("XSH.mbstowcs.008");
	use_utf8();
	pc_test();
	r = mbstowcs(w, s, 8);
	pc_check(r == 2, "mbstowcs() returned %zu, expected 2", r);
	pc_pass();
}
