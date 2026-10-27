/*
 * memcmp: equal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcmp.001
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_memcmp)(const void *, const void *, size_t) = memcmp;

int main(void)
{
	static const unsigned char x[6] = { 1, 0, 0x80, 0xff, 7, 0 };
	unsigned char y[6];
	pc_begin("XSH.memcmp.001");
	memcpy(y, x, 6);
	pc_test();
	pc_check(f_memcmp(x, y, 6) == 0, "equal arrays did not compare equal");
	y[0] = 9;
	pc_check(f_memcmp(x, y, 0) == 0, "n 0 did not compare equal");
	pc_pass();
}
