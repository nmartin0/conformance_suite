/*
 * mbsnrtowcs() reads at most nmc bytes and leaves *src just past the last
 * byte processed; reaching the null byte gives a null *src.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.008
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const char s[] = "abcdef";
	mbstate_t st;
	const char *src;
	wchar_t w[10];
	size_t r1, r2, r3, i;

	pc_begin("XSH.mbsrtowcs.008");
	pc_test();
	memset(&st, 0, sizeof st);
	src = s;
	for (i = 0; i < 10; i++)
		w[i] = 77;
	r1 = mbsnrtowcs(w, &src, 3, 10, &st);
	pc_check(r1 == 3, "mbsnrtowcs() with nmc 3 returned %zu, expected 3", r1);
	pc_check(w[0] == L'a' && w[1] == L'b' && w[2] == L'c', "mbsnrtowcs() stored the wrong codes");
	pc_check(w[3] == 77, "mbsnrtowcs() stored a code beyond the nmc bytes");
	pc_check(src == s + 3, "mbsnrtowcs() left *src at the wrong place after nmc bytes");
	memset(&st, 0, sizeof st);
	src = s;
	r2 = mbsnrtowcs(w, &src, 7, 10, &st);
	pc_check(r2 == 6, "mbsnrtowcs() with nmc 7 returned %zu, expected 6", r2);
	pc_check(w[6] == 0, "mbsnrtowcs() did not store the terminating zero");
	pc_check(src == NULL, "mbsnrtowcs() left *src non-null after reaching the null byte");
	memset(&st, 0, sizeof st);
	src = s;
	r3 = mbsnrtowcs(NULL, &src, 3, 1, &st);
	pc_check(r3 == 3, "mbsnrtowcs() with a null dst and nmc 3 returned %zu, expected 3", r3);
	pc_pass();
}
