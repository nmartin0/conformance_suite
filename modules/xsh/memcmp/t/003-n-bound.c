/*
 * memcmp: n bound.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcmp.003
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_memcmp)(const void *, const void *, size_t) = memcmp;

int main(void)
{
	static const char x[4] = { 'a', 'b', 'c', 'X' }, y[4] = { 'a', 'b', 'c', 'Y' };
	pc_begin("XSH.memcmp.003");
	pc_test();
	pc_check(f_memcmp(x, y, 3) == 0, "a byte after n made the arrays differ");
	pc_check(f_memcmp(x, y, 4) < 0, "the fourth byte was not compared");
	pc_pass();
}
