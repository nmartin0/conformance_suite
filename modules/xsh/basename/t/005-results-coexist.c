/*
 * The results of successive basename() calls stay readable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.basename.005
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	char b1[] = "x/aa", b2[] = "y/bb", b3[] = "";
	char *r1, *r2, *r3, *r4;

	pc_begin("XSH.basename.005");
	pc_test();
	r1 = basename(b1);
	r2 = basename(b2);
	r3 = basename(NULL);
	r4 = basename(b3);
	pc_check(r1 != NULL && r2 != NULL && r3 != NULL && r4 != NULL, "a call returned a null pointer");
	pc_check(strcmp(r1, "aa") == 0, "the first result changed to \"%s\"", r1);
	pc_check(strcmp(r2, "bb") == 0, "the second result changed to \"%s\"", r2);
	pc_check(strcmp(r3, ".") == 0, "the result for a null pointer changed to \"%s\"", r3);
	pc_check(strcmp(r4, ".") == 0, "the result for an empty string changed to \"%s\"", r4);
	pc_pass();
}
