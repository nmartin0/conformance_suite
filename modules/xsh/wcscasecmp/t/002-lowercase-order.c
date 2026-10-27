/*
 * In the POSIX locale wcscasecmp() compares after conversion to lowercase.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscasecmp.002
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

int main(void)
{
	int r[8];

	pc_begin("XSH.wcscasecmp.002");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r[0] = wcscasecmp(L"a", L"B");
	r[1] = wcscasecmp(L"B", L"a");
	r[2] = wcscasecmp(L"_", L"A");
	r[3] = wcscasecmp(L"A", L"_");
	r[4] = wcscasecmp(L"Z", L"a");
	r[5] = wcscasecmp(L"ab", L"ABC");
	r[6] = wcscasecmp(L"ABC", L"ab");
	r[7] = wcscasecmp(L"abz", L"ACA");
	pc_check(SIGN(r[0]) == SGN(L'a', L'b'), "\"a\" against \"B\" returned %d", r[0]);
	pc_check(SIGN(r[1]) == SGN(L'b', L'a'), "\"B\" against \"a\" returned %d", r[1]);
	pc_check(SIGN(r[2]) == SGN(L'_', L'a'), "\"_\" against \"A\" returned %d", r[2]);
	pc_check(SIGN(r[3]) == SGN(L'a', L'_'), "\"A\" against \"_\" returned %d", r[3]);
	pc_check(SIGN(r[4]) == SGN(L'z', L'a'), "\"Z\" against \"a\" returned %d", r[4]);
	pc_check(SIGN(r[5]) == -1, "\"ab\" against \"ABC\" returned %d", r[5]);
	pc_check(SIGN(r[6]) == 1, "\"ABC\" against \"ab\" returned %d", r[6]);
	pc_check(SIGN(r[7]) == SGN(L'b', L'c'), "\"abz\" against \"ACA\" returned %d", r[7]);
	pc_pass();
}
