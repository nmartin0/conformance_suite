/*
 * wcscmp() gives the sign of the first difference.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscmp.002
 */
#include "pc.h"
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t abc[] = { L'a', L'b', L'c', 0 }, abd[] = { L'a', L'b', L'd', 0 };
static wchar_t ab[] = { L'a', L'b', 0 }, none[] = { 0 }, a[] = { L'a', 0 }, bigb[] = { L'B', 0 };
static wchar_t abz[] = { L'a', L'b', L'z', 0 }, acA[] = { L'a', L'c', L'A', 0 };

int main(void)
{
	int r[9];

	pc_begin("XSH.wcscmp.002");
	pc_test();
	r[0] = wcscmp(abc, abd);
	r[1] = wcscmp(abd, abc);
	r[2] = wcscmp(ab, abc);
	r[3] = wcscmp(abc, ab);
	r[4] = wcscmp(none, a);
	r[5] = wcscmp(a, none);
	r[6] = wcscmp(a, bigb);
	r[7] = wcscmp(abz, acA);
	r[8] = wcscmp(acA, abz);
	pc_check(SIGN(r[0]) == SGN(L'c', L'd'), "\"abc\" against \"abd\" returned %d", r[0]);
	pc_check(SIGN(r[1]) == SGN(L'd', L'c'), "\"abd\" against \"abc\" returned %d", r[1]);
	pc_check(SIGN(r[2]) == -1, "\"ab\" against \"abc\" returned %d", r[2]);
	pc_check(SIGN(r[3]) == 1, "\"abc\" against \"ab\" returned %d", r[3]);
	pc_check(SIGN(r[4]) == -1, "\"\" against \"a\" returned %d", r[4]);
	pc_check(SIGN(r[5]) == 1, "\"a\" against \"\" returned %d", r[5]);
	pc_check(SIGN(r[6]) == SGN(L'a', L'B'), "\"a\" against \"B\" returned %d", r[6]);
	pc_check(SIGN(r[7]) == SGN(L'b', L'c'), "\"abz\" against \"acA\" returned %d", r[7]);
	pc_check(SIGN(r[8]) == SGN(L'c', L'b'), "\"acA\" against \"abz\" returned %d", r[8]);
	pc_pass();
}
