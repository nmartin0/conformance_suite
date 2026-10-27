/*
 * Only the first equals sign separates.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsubopt.005
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static char k0[] = "ro", k1[] = "rw", k2[] = "name", k3[] = "size";
static char *const keys[] = { k0, k1, k2, k3, NULL };

int main(void)
{
	char s0[] = "name=a=b", s1[] = "size==3";
	char *opt, *val;
	int r;

	pc_begin("XSH.getsubopt.005");
	pc_test();
	opt = s0;
	val = NULL;
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 2, "\"name=a=b\": returned %d, expected 2", r);
	pc_check(val != NULL && strcmp(val, "a=b") == 0, "\"name=a=b\": the value is %s", val ? val : "(null)");
	opt = s1;
	val = NULL;
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 3, "\"size==3\": returned %d, expected 3", r);
	pc_check(val != NULL && strcmp(val, "=3") == 0, "\"size==3\": the value is %s", val ? val : "(null)");
	pc_pass();
}
