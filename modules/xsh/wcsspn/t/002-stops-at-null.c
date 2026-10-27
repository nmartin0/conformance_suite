/*
 * wcsspn() ends its scan at the null code of the first string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsspn.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', 0, L'a', L'b', 0 }, ab[] = { L'a', L'b', 0 };

int main(void)
{
	size_t r;

	pc_begin("XSH.wcsspn.002");
	pc_test();
	r = wcsspn(s, ab);
	pc_check(r == 2, "wcsspn() went past the null code and returned %zu, expected 2", r);
	pc_pass();
}
