/*
 * wcpcpy() copies the string and returns a pointer to the copied null code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscpy.002
 */
#include "pc.h"
#include <wchar.h>

#define DN 320

static wchar_t d[DN], empty[1], one[] = { L'a', 0 }, big[301];

static int untouched(const wchar_t *p, int from)
{
	int i;

	for (i = from; i < DN; i++)
		if (p[i] != L'X')
			return 0;
	return 1;
}

int main(void)
{
	wchar_t *r[3];
	const wchar_t *src[3];
	static const int len[3] = { 0, 1, 300 };
	int i, k, same;

	pc_begin("XSH.wcscpy.002");
	for (i = 0; i < 300; i++)
		big[i] = L'q';
	src[0] = empty;
	src[1] = one;
	src[2] = big;
	pc_test();
	for (k = 0; k < 3; k++) {
		for (i = 0; i < DN; i++)
			d[i] = L'X';
		r[k] = wcpcpy(d, src[k]);
		pc_check(r[k] == d + len[k], "case %d: wcpcpy() did not return a pointer to the copied null code", k);
		same = 1;
		for (i = 0; i < len[k]; i++)
			if (d[i] != src[k][i])
				same = 0;
		pc_check(same, "case %d: the string was not copied", k);
		pc_check(d[len[k]] == 0, "case %d: the null code was not copied", k);
		pc_check(untouched(d, len[k] + 1), "case %d: a code after the null was changed", k);
	}
	pc_pass();
}
