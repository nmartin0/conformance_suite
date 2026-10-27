/*
 * strcmp: stops at terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcmp.004
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strcmp)(const char *, const char *) = strcmp;

int main(void)
{
	static const char x[4] = { 'a', 'b', 0, 'X' }, y[4] = { 'a', 'b', 0, 'Y' };
	pc_begin("XSH.strcmp.004");
	pc_test();
	pc_check(f_strcmp(x, y) == 0, "bytes after the terminator were compared");
	pc_pass();
}
