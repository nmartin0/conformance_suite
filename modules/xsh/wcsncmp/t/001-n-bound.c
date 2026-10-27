/*
 * wcsncmp() compares at most n codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncmp.001
 */
#include "pc.h"
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t s1[] = { L'a', L'b', L'c', L'd', 0 }, s2[] = { L'a', L'b', L'X', L'Y', 0 };

int main(void)
{
	int r2, r3, r0;

	pc_begin("XSH.wcsncmp.001");
	pc_test();
	r2 = wcsncmp(s1, s2, 2);
	r3 = wcsncmp(s1, s2, 3);
	r0 = wcsncmp(s1, s2, 0);
	pc_check(r2 == 0, "n 2 on strings equal in their first two codes returned %d", r2);
	pc_check(SIGN(r3) == SGN(L'c', L'X'), "n 3 returned %d, expected the sign of c against X", r3);
	pc_check(r0 == 0, "n 0 returned %d", r0);
	pc_pass();
}
