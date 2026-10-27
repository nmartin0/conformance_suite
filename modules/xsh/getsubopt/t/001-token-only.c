/*
 * getsubopt() of a lone token.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsubopt.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static char k0[] = "ro", k1[] = "rw", k2[] = "name", k3[] = "size";
static char *const keys[] = { k0, k1, k2, k3, NULL };

int main(void)
{
	char s0[] = "ro", s1[] = "name", s2[] = "size";
	char *opt, *val;
	int r;

	pc_begin("XSH.getsubopt.001");
	pc_test();
	opt = s0;
	val = (char *)"unset";
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 0, "\"ro\": returned %d, expected 0", r);
	pc_check(val == NULL, "\"ro\": the value pointer is not null");
	pc_check(opt == s0 + strlen(s0), "\"ro\": the option pointer is not at the terminating null");
	opt = s1;
	val = (char *)"unset";
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 2, "\"name\": returned %d, expected 2", r);
	pc_check(val == NULL, "\"name\": the value pointer is not null");
	pc_check(opt == s1 + strlen(s1), "\"name\": the option pointer is not at the terminating null");
	opt = s2;
	val = (char *)"unset";
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 3 && val == NULL && opt == s2 + strlen(s2), "\"size\": returned %d", r);
	pc_pass();
}
