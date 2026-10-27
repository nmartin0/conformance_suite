/*
 * strcmp: sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcmp.002
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strcmp)(const char *, const char *) = strcmp;

int main(void)
{
	static const char *const lo[] = { "a", "abc", "ab", "", "Z" };
	static const char *const hi[] = { "b", "abd", "abc", "a", "a" };
	size_t k;
	pc_begin("XSH.strcmp.002");
	pc_test();
	for (k = 0; k < sizeof lo / sizeof lo[0]; k++) {
		pc_check(f_strcmp(lo[k], hi[k]) < 0, "\"%s\" was not less than \"%s\"", lo[k], hi[k]);
		pc_check(f_strcmp(hi[k], lo[k]) > 0, "\"%s\" was not greater than \"%s\"", hi[k], lo[k]);
	}
	pc_pass();
}
