/*
 * strxfrm() returns the length of what it stores.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.001
 */
#include "lc.h"

#define DN 128

int main(void)
{
	char buf[DN];
	size_t r;

	pc_begin("XSH.strxfrm.001");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	memset(buf, 'X', sizeof buf);
	pc_test();
	r = strxfrm(buf, "abc", DN);
	if (r >= DN)
		pc_unresolved("the transformed string needs %zu bytes, more than the %d provided", r, DN);
	pc_check(buf[r] == '\0', "no NUL at index %zu of the result", r);
	pc_check(strlen(buf) == r, "strxfrm() returned %zu but the stored string has length %zu", r, strlen(buf));
	pc_pass();
}
