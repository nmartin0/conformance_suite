/*
 * memchr: null not special.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memchr.003
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memchr)(const void *, int, size_t) = memchr;

int main(void)
{
	static const char s[4] = { 'a', 0, 'b', 0 };
	pc_begin("XSH.memchr.003");
	pc_test();
	pc_check(f_memchr(s, 0, 4) == s + 1, "the zero byte was not found at index 1");
	pc_check(f_memchr(s, 256, 4) == s + 1, "256 did not search for a zero byte");
	pc_check(f_memchr(s, 'b', 4) == s + 2, "the search stopped at a zero byte");
	pc_check(f_memchr(s + 2, 0, 1) == NULL, "a zero byte after the first n bytes was found");
	pc_pass();
}
