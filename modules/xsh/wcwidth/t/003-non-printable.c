/*
 * wcwidth() of a control character in the POSIX locale is -1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcwidth.003
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static const wchar_t ctl[7] = { L'\a', L'\b', L'\t', L'\n', L'\v', L'\f', L'\r' };

int main(void)
{
	int i, r;

	pc_begin("XSH.wcwidth.003");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	/* control: printable characters have a width (not -1) and the null code has width 0, so -1 above is a judgement */
	r = wcwidth(L'a');
	pc_check(r >= 0, "wcwidth(L'a') returned %d, expected a column count", r);
	r = wcwidth(L'\0');
	pc_check(r == 0, "wcwidth(0) returned %d, expected 0", r);
	for (i = 0; i < 7; i++) {
		r = wcwidth(ctl[i]);
		pc_check(r == -1, "wcwidth() of control character %#x returned %d", (unsigned)ctl[i], r);
	}
	pc_pass();
}
