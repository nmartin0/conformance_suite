/*
 * getsubopt() of token=value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsubopt.002
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static char k0[] = "ro", k1[] = "rw", k2[] = "name", k3[] = "size";
static char *const keys[] = { k0, k1, k2, k3, NULL };

int main(void)
{
	char s0[] = "name=foo", s1[] = "size=", s2[] = "rw=on";
	char *opt, *val;
	int r;

	pc_begin("XSH.getsubopt.002");
	pc_test();
	opt = s0;
	val = NULL;
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 2, "\"name=foo\": returned %d, expected 2", r);
	pc_check(val != NULL && strcmp(val, "foo") == 0, "\"name=foo\": the value is %s", val ? val : "(null)");
	pc_check(val == s0 + 5, "\"name=foo\": the value pointer is not at the first character after the equals sign");
	pc_check(opt == s0 + strlen(s0), "\"name=foo\": the option pointer is not at the terminating null");
	opt = s1;
	val = NULL;
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 3, "\"size=\": returned %d, expected 3", r);
	pc_check(val != NULL && *val == '\0', "\"size=\": the value should be an empty string, not null");
	opt = s2;
	val = NULL;
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 1 && val != NULL && strcmp(val, "on") == 0, "\"rw=on\": returned %d with value %s", r, val ? val : "(null)");
	pc_pass();
}
