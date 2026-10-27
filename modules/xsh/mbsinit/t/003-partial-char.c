/*
 * A state that holds the first byte of an unfinished multibyte character is
 * not an initial conversion state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsinit.003
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
	mbstate_t st;
	size_t n;
	int r;

	pc_begin("XSH.mbsinit.003");
	use_utf8();
	memset(&st, 0, sizeof st);
	n = mbrtowc(NULL, "\xC3", 1, &st);
	pc_check(n == (size_t)-2, "setup: mbrtowc() of the first byte of a two-byte character returned %zu", n);
	pc_test();
	r = mbsinit(&st);
	pc_check(r == 0, "mbsinit() returned non-zero for a state holding an unfinished character");
	pc_pass();
}
