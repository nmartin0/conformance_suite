/*
 * wmemcmp() goes past null codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcmp.003
 */
#include "pc.h"
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t x[] = { L'a', 0, L'b' }, y[] = { L'a', 0, L'c' }, z[] = { L'a', 0, L'b' };

int main(void)
{
	int r1, r2;

	pc_begin("XSH.wmemcmp.003");
	pc_test();
	r1 = wmemcmp(x, y, 3);
	r2 = wmemcmp(x, z, 3);
	pc_check(SIGN(r1) == SGN(L'b', L'c'), "{a,null,b} against {a,null,c} returned %d", r1);
	pc_check(r2 == 0, "{a,null,b} against an equal array returned %d", r2);
	pc_pass();
}
