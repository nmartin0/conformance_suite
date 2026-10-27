/*
 * mbstowcs() converts a string and stores the terminating zero code when
 * there is room.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.001
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	wchar_t w[8];
	size_t r, i;

	pc_begin("XSH.mbstowcs.001");
	for (i = 0; i < 8; i++)
		w[i] = 77;
	pc_test();
	r = mbstowcs(w, "abc", 8);
	pc_check(r == 3, "mbstowcs() returned %zu, expected 3", r);
	pc_check(w[0] == L'a' && w[1] == L'b' && w[2] == L'c', "mbstowcs() stored the wrong codes");
	pc_check(w[3] == 0, "mbstowcs() stored %ld after the codes, expected the terminating zero", (long)w[3]);
	pc_pass();
}
