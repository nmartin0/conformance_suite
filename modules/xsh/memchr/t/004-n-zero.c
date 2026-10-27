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
	pc_check(f_memchr(s, 'a', 0) == NULL, "n 0 found a byte");
	pc_check(f_memchr(s, 0, 0) == NULL, "n 0 found a zero byte");
	pc_pass();
}
