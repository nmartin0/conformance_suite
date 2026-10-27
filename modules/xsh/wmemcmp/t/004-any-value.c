/*
 * wmemcmp() treats every wchar_t value alike.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcmp.004
 */
#include "pc.h"
#include <wchar.h>

#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t big[] = { WCHAR_MAX }, one[] = { 1 }, zero[] = { 0 };
static wchar_t s1[] = { 1, WCHAR_MAX, 0 }, s2[] = { 1, WCHAR_MAX, 0 };

int main(void)
{
	int r1, r2, r3;

	pc_begin("XSH.wmemcmp.004");
	pc_test();
	r1 = wmemcmp(big, one, 1);
	r2 = wmemcmp(zero, big, 1);
	r3 = wmemcmp(s1, s2, 3);
	pc_check(SIGN(r1) == 1, "{WCHAR_MAX} against {1} returned %d", r1);
	pc_check(SIGN(r2) == -1, "{null} against {WCHAR_MAX} returned %d", r2);
	pc_check(r3 == 0, "equal sequences containing WCHAR_MAX returned %d", r3);
	pc_pass();
}
