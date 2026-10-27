/*
 * wmemcmp() gives the sign of the first difference among the first n elements.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcmp.002
 */
#include "pc.h"
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t abc[] = { L'a', L'b', L'c' }, abd[] = { L'a', L'b', L'd' };
static wchar_t az[] = { L'a', L'z' }, ba[] = { L'b', L'a' };

int main(void)
{
	int r[5];

	pc_begin("XSH.wmemcmp.002");
	pc_test();
	r[0] = wmemcmp(abc, abd, 3);
	r[1] = wmemcmp(abd, abc, 3);
	r[2] = wmemcmp(abc, abd, 2);
	r[3] = wmemcmp(az, ba, 2);
	r[4] = wmemcmp(ba, az, 2);
	pc_check(SIGN(r[0]) == SGN(L'c', L'd'), "{a,b,c} against {a,b,d} returned %d", r[0]);
	pc_check(SIGN(r[1]) == SGN(L'd', L'c'), "{a,b,d} against {a,b,c} returned %d", r[1]);
	pc_check(r[2] == 0, "a difference beyond n was taken into account (returned %d)", r[2]);
	pc_check(SIGN(r[3]) == SGN(L'a', L'b'), "{a,z} against {b,a} returned %d", r[3]);
	pc_check(SIGN(r[4]) == SGN(L'b', L'a'), "{b,a} against {a,z} returned %d", r[4]);
	pc_pass();
}
