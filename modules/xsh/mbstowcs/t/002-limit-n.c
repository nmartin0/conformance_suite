/*
 * mbstowcs() modifies no more than n elements, and stores no terminating
 * zero when it returns n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.002
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	wchar_t w[8];
	size_t r1, r2, i;

	pc_begin("XSH.mbstowcs.002");
	pc_test();
	for (i = 0; i < 8; i++)
		w[i] = 77;
	r1 = mbstowcs(w, "abcd", 2);
	pc_check(r1 == 2, "mbstowcs(w, \"abcd\", 2) returned %zu, expected 2", r1);
	pc_check(w[0] == L'a' && w[1] == L'b', "mbstowcs() stored the wrong codes");
	pc_check(w[2] == 77, "mbstowcs() modified the element after the n-th");
	for (i = 0; i < 8; i++)
		w[i] = 77;
	r2 = mbstowcs(w, "abc", 3);
	pc_check(r2 == 3, "mbstowcs(w, \"abc\", 3) returned %zu, expected 3", r2);
	pc_check(w[3] == 77, "mbstowcs() modified the element after the n-th when the string had exactly n characters");
	pc_pass();
}
