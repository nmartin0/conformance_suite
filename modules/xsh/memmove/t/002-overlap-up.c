/*
 * memmove: overlap up.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.002
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	char b[11] = "0123456789";
	pc_begin("XSH.memmove.002");
	pc_test();
	pc_check(f_memmove(b + 2, b, 6) == b + 2, "the return value was not the destination");
	pc_check(memcmp(b, "0101234589", 11) == 0, "overlapping copy upward gave \"%s\"", b);
	pc_pass();
}
