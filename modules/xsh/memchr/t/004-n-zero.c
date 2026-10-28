/*
 * memchr: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memchr.004
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memchr)(const void *, int, size_t) = memchr;

int main(void)
{
	static const char s[2] = { 'a', 'a' };
	pc_begin("XSH.memchr.004");
	pc_test();
	/* control: with n 1 the byte is found, so the null result for n 0 is the function's doing */
	pc_check(f_memchr(s, 'a', 1) == s, "n 1 did not find the byte at index 0");
	pc_check(f_memchr(s, 'a', 0) == NULL, "n 0 found a byte");
	pc_check(f_memchr(s, 0, 0) == NULL, "n 0 found a zero byte");
	pc_pass();
}
