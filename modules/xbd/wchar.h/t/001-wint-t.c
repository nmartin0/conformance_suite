/*
 * wint_t is an integer type that can hold wide-character values and
 *   WEOF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.001
 */
#include <wchar.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	static const wchar_t sample[] = L"ABCXYZabcxyz0189 !\"#%&'()*+,-./:;<=>?[\\]^_{|}~";
	size_t i;
	wint_t w;

	pc_begin("XBD.wchar.h.001");
	pc_test();
	pc_check((wint_t)1.9 == 1, "wint_t is not an integer type");
	for (i = 0; i < sizeof sample / sizeof sample[0]; i++) {
		w = sample[i];
		pc_check((wchar_t)w == sample[i], "wint_t does not hold the wide character %#lx", (unsigned long)sample[i]);
	}
	w = WEOF;
	pc_check(w == WEOF, "wint_t does not hold WEOF");
	pc_pass();
}
