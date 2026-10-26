/*
 * memccpy() compares bytes with c converted to unsigned char.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memccpy.003
 * MUTANT    memccpy.int_compare
 */
#include "pc.h"
#include <string.h>

static char src[8] = { 'a', 'b', 'c', '\xff', 'd', 'b', 'e', 'f' }, dst[16];

int main(void)
{
	void *r1, *r2;

	pc_begin("XSH.memccpy.003");
	pc_test();
	memset(dst, 'X', sizeof dst);
	r1 = memccpy(dst, src, -1, 8);
	pc_check(r1 == dst + 4, "memccpy(c -1) did not stop after the 0xff byte (%s)",
	    r1 == NULL ? "null pointer" : "another pointer");
	pc_check(dst[4] == 'X', "memccpy(c -1) copied past the 0xff byte");
	memset(dst, 'X', sizeof dst);
	r2 = memccpy(dst, src, 'b' + 256, 8);
	pc_check(r2 == dst + 2, "memccpy(c 'b' + 256) did not stop after the first 'b' (%s)",
	    r2 == NULL ? "null pointer" : "another pointer");
	pc_check(dst[2] == 'X', "memccpy(c 'b' + 256) copied past the 'b'");
	pc_pass();
}
