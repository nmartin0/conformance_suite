/*
 * wcsncasecmp() ignores case and compares at most n codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscasecmp.003
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

static wchar_t s1[] = L"abcd", s2[] = L"ABXY";
static wchar_t x[] = { L'a', L'b', 0, L'x' }, y[] = { L'A', L'B', 0, L'y' };
static wchar_t ab[] = L"ab", abc[] = L"ABC";

int main(void)
{
	int r[6];

	pc_begin("XSH.wcscasecmp.003");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r[0] = wcsncasecmp(s1, s2, 2);
	r[1] = wcsncasecmp(s1, s2, 3);
	r[2] = wcsncasecmp(s1, s2, 0);
	r[3] = wcsncasecmp(x, y, 4);
	r[4] = wcsncasecmp(ab, abc, 5);
	r[5] = wcsncasecmp(abc, ab, 5);
	pc_check(r[0] == 0, "n 2 on strings equal in two codes returned %d", r[0]);
	pc_check(SIGN(r[1]) == SGN(L'c', L'x'), "n 3 returned %d, expected the sign of c against x", r[1]);
	pc_check(r[2] == 0, "n 0 returned %d", r[2]);
	pc_check(r[3] == 0, "codes after the common null code were compared (returned %d)", r[3]);
	pc_check(SIGN(r[4]) == -1, "\"ab\" against \"ABC\" returned %d", r[4]);
	pc_check(SIGN(r[5]) == 1, "\"ABC\" against \"ab\" returned %d", r[5]);
	pc_pass();
}
