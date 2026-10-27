/*
 * Two wcstok() scans, each with its own save pointer, can be interleaved.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.006
 */
#include "pc.h"
#include <wchar.h>

static wchar_t a[] = L"a b c", b[] = L"1,2,3", sa[] = L" ", sb[] = L",";

int main(void)
{
	wchar_t *ra[4], *rb[4], *pa = NULL, *pb = NULL;
	int i;

	pc_begin("XSH.wcstok.006");
	pc_test();
	ra[0] = wcstok(a, sa, &pa);
	rb[0] = wcstok(b, sb, &pb);
	for (i = 1; i < 4; i++) {
		ra[i] = wcstok(NULL, sa, &pa);
		rb[i] = wcstok(NULL, sb, &pb);
	}
	pc_check(ra[0] == a && ra[1] == a + 2 && ra[2] == a + 4, "the tokens of \"a b c\" are not at s, s+2, s+4");
	pc_check(ra[3] == NULL, "the scan of \"a b c\" did not end with a null pointer");
	pc_check(rb[0] == b && rb[1] == b + 2 && rb[2] == b + 4, "the tokens of \"1,2,3\" are not at s, s+2, s+4");
	pc_check(rb[3] == NULL, "the scan of \"1,2,3\" did not end with a null pointer");
	pc_check(wcscmp(a, L"a") == 0 && wcscmp(a + 2, L"b") == 0 && wcscmp(a + 4, L"c") == 0, "the tokens of \"a b c\" are wrong");
	pc_check(wcscmp(b, L"1") == 0 && wcscmp(b + 2, L"2") == 0 && wcscmp(b + 4, L"3") == 0, "the tokens of \"1,2,3\" are wrong");
	pc_pass();
}
