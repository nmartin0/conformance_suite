/*
 * memchr: unsigned conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memchr.002
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memchr)(const void *, int, size_t) = memchr;

int main(void)
{
	static const unsigned char b[5] = { 0x01, 0xe9, 0x7f, 0x80, 0xff };
	pc_begin("XSH.memchr.002");
	pc_test();
	pc_check(f_memchr(b, 489, 5) == b + 1, "489 did not convert to 0xe9");
	pc_check(f_memchr(b, -23, 5) == b + 1, "-23 did not convert to 0xe9");
	pc_check(f_memchr(b, -1, 5) == b + 4, "-1 did not convert to 0xff");
	pc_check(f_memchr(b, 255, 5) == b + 4, "255 did not find 0xff");
	pc_check(f_memchr(b, 128, 5) == b + 3, "128 did not find 0x80");
	pc_check(f_memchr(b, 0x17f, 5) == b + 2, "0x17f did not convert to 0x7f");
	pc_check(f_memchr(b, 257, 5) == b, "257 did not convert to 1");
	pc_check(f_memchr(b, 256, 5) == NULL, "256 did not convert to 0 (no zero byte present)");
	pc_pass();
}
