/*
 * strcmp: equal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcmp.001
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strcmp)(const char *, const char *) = strcmp;

int main(void)
{
	static const char x[] = "abc", y[] = "abc", e1[] = "", e2[] = "", h1[] = { (char)0xe9, (char)0xff, 0 }, h2[] = { (char)0xe9, (char)0xff, 0 };
	pc_begin("XSH.strcmp.001");
	pc_test();
	pc_check(f_strcmp(x, y) == 0, "equal strings did not compare equal");
	pc_check(f_strcmp(e1, e2) == 0, "empty strings did not compare equal");
	pc_check(f_strcmp(h1, h2) == 0, "equal strings with high bytes did not compare equal");
	pc_pass();
}
