/*
 * With a null dst mbsrtowcs() counts the characters whatever len is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.003
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	const char *src = "abcd";
	size_t r1, r2;

	pc_begin("XSH.mbsrtowcs.003");
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = mbsrtowcs(NULL, &src, 1, &st);
	src = "abcd";
	r2 = mbsrtowcs(NULL, &src, 100, &st);
	pc_check(r1 == 4, "mbsrtowcs(NULL, &src, 1, &st) returned %zu, expected 4", r1);
	pc_check(r2 == 4, "mbsrtowcs(NULL, &src, 100, &st) returned %zu, expected 4", r2);
	pc_pass();
}
