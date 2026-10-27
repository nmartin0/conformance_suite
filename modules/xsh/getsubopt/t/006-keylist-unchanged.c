/*
 * getsubopt() leaves the key vector alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsubopt.006
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static char k0[] = "ro", k1[] = "rw", k2[] = "name", k3[] = "size";
static char *const keys[] = { k0, k1, k2, k3, NULL };

int main(void)
{
	char s[] = "ro,name=x,bogus,size=1,rw";
	char *opt = s, *val;
	char *saved[5];
	char text[4][8];
	int i;

	pc_begin("XSH.getsubopt.006");
	for (i = 0; i < 5; i++)
		saved[i] = keys[i];
	for (i = 0; i < 4; i++)
		strcpy(text[i], keys[i]);
	pc_test();
	while (*opt != '\0')
		(void)getsubopt(&opt, keys, &val);
	for (i = 0; i < 5; i++)
		pc_check(keys[i] == saved[i], "key vector entry %d changed", i);
	for (i = 0; i < 4; i++)
		pc_check(strcmp(keys[i], text[i]) == 0, "key string %d changed to \"%s\"", i, keys[i]);
	pc_pass();
}
