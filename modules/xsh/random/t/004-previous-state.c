/*
 * initstate() and setstate() return the previous state array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.004
 */
#include "pc.h"
#include <stdlib.h>

static long sa[256 / sizeof(long) + 1], sb[256 / sizeof(long) + 1];

int main(void)
{
	char *r;

	pc_begin("XSH.random.004");
	pc_test();
	r = initstate(3, (char *)sa, 128);
	pc_check(r != NULL, "the first initstate() returned a null pointer");
	r = initstate(4, (char *)sb, 64);
	pc_check(r == (char *)sa, "the second initstate() returned %p, expected the first array %p", (void *)r, (void *)sa);
	r = setstate((char *)sa);
	pc_check(r == (char *)sb, "setstate(A) returned %p, expected the array in use, B (%p)", (void *)r, (void *)sb);
	r = setstate((char *)sb);
	pc_check(r == (char *)sa, "setstate(B) returned %p, expected A (%p)", (void *)r, (void *)sa);
	pc_pass();
}
