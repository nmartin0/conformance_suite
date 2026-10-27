/*
 * memmem: zero bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmem.002
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmem)(const void *, size_t, const void *, size_t) = memmem;

int main(void)
{
	static const unsigned char h[5] = { 1, 0, 2, 0, 3 };
	static const unsigned char n1[2] = { 0, 2 }, n2[2] = { 0, 3 }, n3[1] = { 0 }, n4[2] = { 0, 0 }, n5[3] = { 1, 0, 2 };
	pc_begin("XSH.memmem.002");
	pc_test();
	pc_check(f_memmem(h, 5, n1, 2) == h + 1, "{0, 2} not found at index 1");
	pc_check(f_memmem(h, 5, n2, 2) == h + 3, "{0, 3} not found at index 3");
	pc_check(f_memmem(h, 5, n3, 1) == h + 1, "{0} not found at index 1");
	pc_check(f_memmem(h, 5, n4, 2) == NULL, "{0, 0} was found");
	pc_check(f_memmem(h, 5, n5, 3) == h, "{1, 0, 2} not found at index 0");
	pc_pass();
}
