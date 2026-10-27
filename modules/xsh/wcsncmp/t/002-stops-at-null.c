/*
 * wcsncmp() does not compare codes after a null code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncmp.002
 */
#include "pc.h"
#include <wchar.h>

#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t x[] = { L'a', L'b', 0, L'x' }, y[] = { L'a', L'b', 0, L'y' };
static wchar_t ab[] = { L'a', L'b', 0 }, abc[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	int r0, r1, r2;

	pc_begin("XSH.wcsncmp.002");
	pc_test();
	r0 = wcsncmp(x, y, 4);
	r1 = wcsncmp(ab, abc, 5);
	r2 = wcsncmp(abc, ab, 5);
	pc_check(r0 == 0, "codes after the common null code were compared (returned %d)", r0);
	pc_check(SIGN(r1) == -1, "\"ab\" against \"abc\" returned %d", r1);
	pc_check(SIGN(r2) == 1, "\"abc\" against \"ab\" returned %d", r2);
	pc_pass();
}
