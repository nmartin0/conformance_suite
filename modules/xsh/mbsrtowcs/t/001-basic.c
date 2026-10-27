/*
 * mbsrtowcs() converts up to and including the terminating null byte, sets
 * *src to a null pointer and leaves the initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.001
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const char s[] = { 'a', 'b', 'c', '\0', 'z', 'z', '\0' };
	mbstate_t st;
	const char *src = s;
	wchar_t w[8];
	size_t r, i;
	int init;

	pc_begin("XSH.mbsrtowcs.001");
	memset(&st, 0, sizeof st);
	for (i = 0; i < 8; i++)
		w[i] = 77;
	pc_test();
	r = mbsrtowcs(w, &src, 8, &st);
	init = mbsinit(&st);
	pc_check(r == 3, "mbsrtowcs() returned %zu, expected 3", r);
	pc_check(w[0] == L'a' && w[1] == L'b' && w[2] == L'c', "mbsrtowcs() stored the wrong codes");
	pc_check(w[3] == 0, "mbsrtowcs() stored %ld after the codes, expected the terminating zero", (long)w[3]);
	pc_check(src == NULL, "mbsrtowcs() left *src non-null after reaching the null byte");
	pc_check(init != 0, "mbsrtowcs() left a state that is not initial after reaching the null byte");
	pc_pass();
}
