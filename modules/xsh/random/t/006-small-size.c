/*
 * initstate() with a state array that is too small.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.006
 */
#include "pc.h"
#include <stdlib.h>

static long st[64];

int main(void)
{
	size_t n;

	pc_begin("XSH.random.006");
	pc_test();
	/* a size the function accepts gives a pointer, so a null result below is the small-size refusal */
	pc_check(initstate(5, (char *)st, 128) != NULL, "initstate() with a size of 128 returned a null pointer");
	for (n = 0; n < 8; n++)
		pc_check(initstate(5, (char *)st, n) == NULL, "initstate() with size %lu did not return a null pointer", (unsigned long)n);
	pc_pass();
}
