/*
 * memmem: bound.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmem.005
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmem)(const void *, size_t, const void *, size_t) = memmem;

int main(void)
{
	static const char h[5] = "abcd";
	pc_begin("XSH.memmem.005");
	pc_test();
	pc_check(f_memmem(h, 3, "cd", 2) == NULL, "a match extending past haystacklen was found");
	pc_check(f_memmem(h, 3, "bc", 2) == h + 1, "a match ending at haystacklen was not found");
	pc_check(f_memmem(h, 3, "d", 1) == NULL, "a byte past haystacklen was found");
	pc_pass();
}
