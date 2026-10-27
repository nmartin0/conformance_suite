/*
 * wcsdup() returns a new array with the same content, which free() accepts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsdup.001
 */
#include "pc.h"
#include <stdlib.h>
#include <wchar.h>

static wchar_t empty[1], abc[] = { L'a', L'b', L'c', 0 }, big[1001];

int main(void)
{
	const wchar_t *src[3];
	static const int len[3] = { 0, 3, 1000 };
	wchar_t *r[3];
	int i, k, same;

	pc_begin("XSH.wcsdup.001");
	for (i = 0; i < 1000; i++)
		big[i] = (wchar_t)(L'a' + i % 26);
	src[0] = empty;
	src[1] = abc;
	src[2] = big;
	pc_test();
	for (k = 0; k < 3; k++)
		r[k] = wcsdup(src[k]);
	for (k = 0; k < 3; k++) {
		if (r[k] == NULL)
			pc_unresolved("wcsdup() returned a null pointer (memory may have run out)");
		pc_check(r[k] != src[k], "case %d: wcsdup() returned the argument itself", k);
		same = 1;
		for (i = 0; i <= len[k]; i++)
			if (r[k][i] != src[k][i])
				same = 0;
		pc_check(same, "case %d: the duplicate differs from the original", k);
	}
	for (k = 0; k < 3; k++)
		free(r[k]);
	pc_pass();
}
