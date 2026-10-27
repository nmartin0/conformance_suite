/*
 * getsubopt() with no matching token.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsubopt.004
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static char k0[] = "ro", k1[] = "rw", k2[] = "name", k3[] = "size";
static char *const keys[] = { k0, k1, k2, k3, NULL };

int main(void)
{
	char s[] = "nam,names,bogus,ro,r,rwx";
	char *opt = s, *val;
	static const int expect[] = { -1, -1, -1, 0, -1, -1 };
	int i;

	pc_begin("XSH.getsubopt.004");
	pc_test();
	for (i = 0; i < 6; i++) {
		int r;

		pc_check(*opt != '\0', "the option pointer reached the end after %d suboptions", i);
		r = getsubopt(&opt, keys, &val);
		pc_check(r == expect[i], "suboption %d returned %d, expected %d", i, r, expect[i]);
	}
	pc_check(*opt == '\0', "the option pointer did not reach the end of the string");
	pc_pass();
}
