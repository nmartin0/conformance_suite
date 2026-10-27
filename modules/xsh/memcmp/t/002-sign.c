/*
 * memcmp: sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcmp.002
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_memcmp)(const void *, const void *, size_t) = memcmp;

int main(void)
{
	static const unsigned char a1[1] = { 0x7f }, a2[1] = { 0x80 }, a3[1] = { 0x01 }, a4[1] = { 0xff }, a5[1] = { 0x00 };
	static const unsigned char b1[2] = { 1, 9 }, b2[2] = { 2, 0 };
	static const unsigned char c1[3] = { 'a', 0, 'c' }, c2[3] = { 'a', 0, 'd' };
	pc_begin("XSH.memcmp.002");
	pc_test();
	pc_check(f_memcmp(a1, a2, 1) < 0, "0x7f was not less than 0x80");
	pc_check(f_memcmp(a2, a1, 1) > 0, "0x80 was not greater than 0x7f");
	pc_check(f_memcmp(a2, a3, 1) > 0, "0x80 was not greater than 0x01");
	pc_check(f_memcmp(a4, a5, 1) > 0, "0xff was not greater than 0x00");
	pc_check(f_memcmp(a5, a4, 1) < 0, "0x00 was not less than 0xff");
	pc_check(f_memcmp(b1, b2, 2) < 0, "a later byte decided over the first differing one");
	pc_check(f_memcmp(b2, b1, 2) > 0, "a later byte decided over the first differing one (reversed)");
	pc_check(f_memcmp(c1, c2, 3) < 0, "a zero byte ended the comparison");
	pc_pass();
}
