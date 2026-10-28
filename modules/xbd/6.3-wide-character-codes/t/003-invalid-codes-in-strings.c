/*
 * The wide-character string and memory functions treat array elements that
 * are not valid wide-character codes of the locale by their specified
 * semantics: lengths, copies, comparisons and searches work on the values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.3-wide-character-codes.003
 */
#include "pc.h"
#include <limits.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

static int invalid(wchar_t v)
{
	char buf[MB_LEN_MAX];
	mbstate_t st;

	memset(&st, 0, sizeof st);
	return wcrtomb(buf, v, &st) == (size_t)-1;
}

int main(void)
{
	static const long cand[] = { 0x7fffffffL, 0x40000000L, 0x1000000L, 0x200000L, 0x110000L, 0x30000L, 0x10000L, 0x1000L };
	wchar_t bad1 = 0, bad2 = 0;
	wchar_t a[6], b[6], c[6];
	size_t i;

	pc_begin("XBD.6.3-wide-character-codes.003");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	for (i = 0; i < sizeof cand / sizeof cand[0]; i++) {
		wchar_t v = (wchar_t)cand[i];

		if (v != cand[i] || v <= 0 || !invalid(v))
			continue;
		if (bad1 == 0)
			bad1 = v;
		else if (v != bad1) {
			bad2 = v;
			break;
		}
	}
	if (bad1 == 0 || bad2 == 0)
		pc_unresolved("no two wide-character values that the POSIX locale rejects were found");
	a[0] = L'a'; a[1] = bad1; a[2] = L'b'; a[3] = bad2; a[4] = L'c'; a[5] = 0;
	pc_test();
	pc_check(wcslen(a) == 5, "wcslen of a string holding invalid codes is %lu, not 5", (unsigned long)wcslen(a));
	pc_check(wcscpy(b, a) == b && wcscmp(a, b) == 0 && b[1] == bad1 && b[3] == bad2, "wcscpy did not copy the values");
	memset(c, 0, sizeof c);
	pc_check(wcsncpy(c, a, 6) == c && wmemcmp(a, c, 6) == 0, "wcsncpy did not copy the values");
	memset(c, 0, sizeof c);
	pc_check(wmemcpy(c, a, 6) == c && wmemcmp(a, c, 6) == 0, "wmemcpy did not copy the values");
	pc_check(wcschr(a, bad2) == a + 3, "wcschr did not find an invalid code");
	pc_check(wcsrchr(a, bad1) == a + 1, "wcsrchr did not find an invalid code");
	pc_check(wmemchr(a, bad2, 6) == a + 3, "wmemchr did not find an invalid code");
	pc_check(wcschr(a, 0) == a + 5, "wcschr did not find the terminator");
	b[3] = bad1;
	pc_check(wcscmp(a, b) != 0 && wcsncmp(a, b, 5) != 0 && wmemcmp(a, b, 6) != 0,
	    "strings that differ in one invalid code compare equal");
	pc_check(wcsncmp(a, b, 3) == 0, "wcsncmp looked beyond n at an invalid code");
	{
		wchar_t set[3];

		set[0] = L'a'; set[1] = bad1; set[2] = 0;
		pc_check(wcsspn(a, set) == 2, "wcsspn with an invalid code in the set gave %lu", (unsigned long)wcsspn(a, set));
		set[0] = bad2; set[1] = 0;
		pc_check(wcscspn(a, set) == 3, "wcscspn with an invalid code in the set gave %lu", (unsigned long)wcscspn(a, set));
		pc_check(wcspbrk(a, set) == a + 3, "wcspbrk did not find an invalid code");
	}
	memset(c, 0, sizeof c);
	pc_check(wmemset(c, bad1, 4) == c && c[0] == bad1 && c[3] == bad1 && c[4] == 0, "wmemset did not fill with the value");
	pc_pass();
}
