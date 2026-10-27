/*
 * memchr: first match.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memchr.001
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memchr)(const void *, int, size_t) = memchr;

int main(void)
{
	static const char s[] = "abcabc";
	pc_begin("XSH.memchr.001");
	pc_test();
	pc_check(f_memchr(s, 'b', 6) == s + 1, "first b not found at index 1");
	pc_check(f_memchr(s, 'a', 6) == s, "first a not found at index 0");
	pc_check(f_memchr(s, 'z', 6) == NULL, "absent byte was reported present");
	pc_check(f_memchr(s, 'c', 2) == NULL, "byte at index n was considered");
	pc_check(f_memchr(s, 'c', 3) == s + 2, "byte at index n-1 was not found");
	pc_check(f_memchr(s + 3, 'a', 3) == s + 3, "search from an inner pointer failed");
	pc_pass();
}
