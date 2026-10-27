/*
 * memmove: overlap sweep.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.005
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	static const size_t sizes[] = { 0, 1, 2, 5, 8, 13, 16 };
	unsigned char a[64], ref[64], tmp[16];
	size_t k, i;
	int so, d_o;
	pc_begin("XSH.memmove.005");
	pc_test();
	for (k = 0; k < sizeof sizes / sizeof sizes[0]; k++)
		for (so = 0; so < 16; so++)
			for (d_o = 0; d_o < 16; d_o++) {
				size_t n = sizes[k];
				for (i = 0; i < 64; i++)
					a[i] = ref[i] = (unsigned char)(i * 5 + 1);
				for (i = 0; i < n; i++)
					tmp[i] = ref[16 + so + i];
				for (i = 0; i < n; i++)
					ref[16 + d_o + i] = tmp[i];
				pc_check(f_memmove(a + 16 + d_o, a + 16 + so, n) == a + 16 + d_o, "wrong return value (n %lu, src %d, dst %d)", (unsigned long)n, so, d_o);
				for (i = 0; i < 64; i++)
					if (a[i] != ref[i])
						pc_fail("n %lu, src offset %d, dst offset %d: byte %lu is wrong", (unsigned long)n, so, d_o, (unsigned long)i);
			}
	pc_pass();
}
