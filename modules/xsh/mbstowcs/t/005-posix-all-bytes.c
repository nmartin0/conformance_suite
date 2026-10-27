/*
 * In the POSIX locale every non-null byte is a valid character, so a string
 * of all of them converts without error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.005
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	char s[256];
	wchar_t w[256];
	size_t r, i;
	int zero = 0;

	pc_begin("XSH.mbstowcs.005");
	for (i = 0; i < 255; i++)
		s[i] = (char)(i + 1);
	s[255] = '\0';
	pc_test();
	r = mbstowcs(w, s, 256);
	pc_check(r == 255, "mbstowcs() of the 255 non-null bytes returned %zu, expected 255", r);
	for (i = 0; i < 255; i++)
		if (w[i] == 0)
			zero = 1;
	pc_check(!zero, "mbstowcs() converted a non-null byte to the null code");
	pc_check(w[255] == 0, "mbstowcs() did not store the terminating zero");
	pc_pass();
}
