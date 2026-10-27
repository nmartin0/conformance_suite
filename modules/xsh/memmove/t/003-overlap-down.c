/*
 * memmove: overlap down.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.003
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	char b[11] = "0123456789";
	pc_begin("XSH.memmove.003");
	pc_test();
	pc_check(f_memmove(b, b + 2, 6) == b, "the return value was not the destination");
	pc_check(memcmp(b, "2345676789", 11) == 0, "overlapping copy downward gave \"%s\"", b);
	pc_pass();
}
