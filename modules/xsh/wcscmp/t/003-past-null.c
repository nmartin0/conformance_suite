/*
 * wcscmp() does not look at codes after the common null code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscmp.003
 */
#include "pc.h"
#include <wchar.h>

static wchar_t x[] = { L'a', L'b', 0, L'x' }, y[] = { L'a', L'b', 0, L'y' };

int main(void)
{
	int r;

	pc_begin("XSH.wcscmp.003");
	pc_test();
	r = wcscmp(x, y);
	pc_check(r == 0, "codes after the common null code were compared (returned %d)", r);
	pc_pass();
}
