/*
 * Arguments are pushed right to left on the stack, each at its natural alignment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.013
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.013")
#else
struct s5 { char c[5]; };
ABI_ASM_CAPTURE_ARGS(abi_capture);
void abi_capture(unsigned *out, int a, double d, char c, short sh, long long ll, float f, struct s5 s,
    long double ld, int last);
int main(void)
{
	pc_begin("ABI.i386.calling.013");
	unsigned out[17];
	struct s5 s = { { 'a', 'b', 'c', 'd', 'e' } };
	const unsigned char *b;
	unsigned char ld[10];
	long double v = 3.25L;

	memset(out, 0xEE, sizeof out);
	pc_test();
	abi_capture(out, 0x11111111, 2.5, 'A', (short)-2, 0x0123456789ABCDEFLL, 1.5f, s, v, 0x22222222);
	pc_check(out[0] == 0x11111111u, "int argument at word 0 is 0x%08x", out[0]);
	pc_check(out[1] == 0 && out[2] == 0x40040000u, "double 2.5 occupies words 1-2 as 0x%08x 0x%08x", out[1], out[2]);
	pc_check((out[3] & 0xFFu) == 'A', "char argument at word 3 is 0x%08x", out[3]);
	pc_check((out[4] & 0xFFFFu) == 0xFFFEu, "short argument at word 4 is 0x%08x", out[4]);
	pc_check(out[5] == 0x89ABCDEFu && out[6] == 0x01234567u, "long long occupies words 5-6 as 0x%08x 0x%08x", out[5], out[6]);
	pc_check(out[7] == 0x3FC00000u, "float 1.5 at word 7 is 0x%08x", out[7]);
	b = (const unsigned char *)&out[8];
	pc_check(memcmp(b, "abcde", 5) == 0, "the 5-byte structure argument is not in words 8-9");
	memcpy(ld, &out[10], 10);
	pc_check(memcmp(ld, &v, 10) == 0, "long double occupies words 10-12 (80-bit value)");
	pc_check(out[13] == 0x22222222u, "the last int argument at word 13 is 0x%08x (the structure takes 2 words, the long double 3)", out[13]);
	pc_pass();
}
#endif
