/*
 * wcscspn() ends its scan at the null code of the first string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscspn.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', 0, L'c', L'd', 0 }, c[] = { L'c', 0 };

int main(void)
{
	size_t r;

	pc_begin("XSH.wcscspn.002");
	pc_test();
	r = wcscspn(s, c);
	pc_check(r == 2, "wcscspn() went past the null code and returned %zu, expected 2", r);
	pc_pass();
}
