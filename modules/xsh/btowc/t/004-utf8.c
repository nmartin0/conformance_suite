/*
 * In a UTF-8 locale btowc() accepts an ASCII byte and refuses bytes that are
 * not complete characters by themselves.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.btowc.004
 */
#include "pc.h"
#include <locale.h>
#include <stdio.h>
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
	static const int bad[] = { 0x80, 0xC3, 0xFF };
	wint_t r;
	size_t i;

	pc_begin("XSH.btowc.004");
	use_utf8();
	pc_test();
	r = btowc('A');
	pc_check(r == (wint_t)'A', "btowc('A') returned %lu", (unsigned long)r);
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		r = btowc(bad[i]);
		pc_check(r == WEOF, "btowc(0x%X) returned %lu, expected WEOF", bad[i], (unsigned long)r);
	}
	pc_pass();
}
