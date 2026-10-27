/*
 * memmove: same pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.004
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	char b[6] = "abcde";
	pc_begin("XSH.memmove.004");
	pc_test();
	pc_check(f_memmove(b, b, 5) == b, "the return value was not the destination");
	pc_check(memcmp(b, "abcde", 6) == 0, "the bytes changed");
	pc_pass();
}
