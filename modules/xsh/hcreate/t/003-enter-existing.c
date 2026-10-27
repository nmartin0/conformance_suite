/*
 * hsearch(ENTER) of an existing key.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.hcreate.003
 */
#include "pc.h"
#include <search.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	static char a[] = "alpha", b[] = "beta", a2[] = "alpha";
	static int d1 = 1, d2 = 2, d3 = 3;
	ENTRY e, *r1, *r2, *rf;

	pc_begin("XSH.hcreate.003");
	pc_check(hcreate(10) != 0, "setup: hcreate() failed");
	e.key = a;
	e.data = &d1;
	pc_check(hsearch(e, ENTER) != NULL, "setup: ENTER failed");
	e.key = b;
	e.data = &d2;
	pc_check(hsearch(e, ENTER) != NULL, "setup: ENTER failed");
	pc_test();
	e.key = a;
	e.data = NULL;
	rf = hsearch(e, FIND);
	pc_check(rf != NULL, "alpha not found");
	e.key = a2;
	e.data = &d3;
	r1 = hsearch(e, ENTER);
	pc_check(r1 != NULL, "ENTER of an existing key returned a null pointer");
	pc_check(r1 == rf, "ENTER of an existing key returned a location different from FIND's");
	r2 = hsearch(e, ENTER);
	pc_check(r2 == r1, "a repeated ENTER returned a different location");
	pc_check(strcmp(r1->key, "alpha") == 0, "the entry has key %s", r1->key);
	e.key = b;
	rf = hsearch(e, FIND);
	pc_check(rf != NULL && rf->data == &d2, "the other entry was disturbed");
	hdestroy();
	pc_pass();
}
