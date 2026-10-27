/*
 * wcsncmp() gives the sign of the first differing pair.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncmp.003
 */
#include "pc.h"
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t abz[] = { L'a', L'b', L'z', 0 }, acA[] = { L'a', L'c', L'A', 0 };

int main(void)
{
	int f, b;

	pc_begin("XSH.wcsncmp.003");
	pc_test();
	f = wcsncmp(abz, acA, 3);
	b = wcsncmp(acA, abz, 3);
	pc_check(SIGN(f) == SGN(L'b', L'c'), "\"abz\" against \"acA\" returned %d", f);
	pc_check(SIGN(b) == SGN(L'c', L'b'), "\"acA\" against \"abz\" returned %d", b);
	pc_pass();
}
