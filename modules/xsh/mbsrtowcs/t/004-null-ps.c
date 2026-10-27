/*
 * mbsrtowcs() with a null ps uses its own state and converts in the POSIX
 * locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.004
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	const char *src = "abc";
	wchar_t w[8];
	size_t r;

	pc_begin("XSH.mbsrtowcs.004");
	pc_test();
	r = mbsrtowcs(w, &src, 8, NULL);
	pc_check(r == 3, "mbsrtowcs() with a null ps returned %zu, expected 3", r);
	pc_check(w[0] == L'a' && w[1] == L'b' && w[2] == L'c' && w[3] == 0, "mbsrtowcs() with a null ps stored the wrong codes");
	pc_pass();
}
