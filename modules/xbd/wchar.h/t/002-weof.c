/*
 * WEOF is a constant expression of type wint_t that is not the code of
 *   any portable character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.002
 */
#include <wchar.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

static const wint_t eof_value = WEOF;

int main(void)
{
	static const wchar_t sample[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 \t\n!\"#%&'()*+,-./:;<=>?[\\]^_{|}~";
	size_t i;

	pc_begin("XBD.wchar.h.002");
	pc_test();
	pc_check(IS_T(WEOF, wint_t), "WEOF does not have type wint_t");
	pc_check((wint_t)L'\0' != eof_value, "WEOF equals the null wide character");
	for (i = 0; i < sizeof sample / sizeof sample[0]; i++)
		pc_check((wint_t)sample[i] != eof_value, "WEOF equals the wide character %#lx", (unsigned long)sample[i]);
	pc_pass();
}
