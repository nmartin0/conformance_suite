/*
 * getsubopt() over a comma-separated list.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsubopt.003
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static char k0[] = "ro", k1[] = "rw", k2[] = "name", k3[] = "size";
static char *const keys[] = { k0, k1, k2, k3, NULL };

int main(void)
{
	char s[] = "ro,name=x,size=10,rw";
	char *opt = s, *val;
	char *start1, *start2, *start3;
	int r;

	pc_begin("XSH.getsubopt.003");
	pc_test();
	start1 = s + 3;
	start2 = s + 10;
	start3 = s + 18;
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 0 && val == NULL, "first: %d", r);
	pc_check(opt == start1, "after \"ro\" the option pointer is not at \"name=x\"");
	pc_check(s[2] == '\0', "the comma after \"ro\" was not replaced by a null character");
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 2 && val != NULL && strcmp(val, "x") == 0, "second: %d, value %s", r, val ? val : "(null)");
	pc_check(opt == start2, "after \"name=x\" the option pointer is not at \"size=10\"");
	pc_check(s[9] == '\0', "the comma after \"name=x\" was not replaced by a null character");
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 3 && val != NULL && strcmp(val, "10") == 0, "third: %d, value %s", r, val ? val : "(null)");
	pc_check(opt == start3, "after \"size=10\" the option pointer is not at \"rw\"");
	r = getsubopt(&opt, keys, &val);
	pc_check(r == 1 && val == NULL, "fourth: %d", r);
	pc_check(opt == s + strlen("ro,name=x,size=10,rw"), "at the end the option pointer is not at the terminating null");
	pc_pass();
}
