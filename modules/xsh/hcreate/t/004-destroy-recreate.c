/*
 * hdestroy() followed by hcreate().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.hcreate.004
 */
#include "pc.h"
#include <search.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	static char keys[3][8] = { "one", "two", "three" };
	static int data[3] = { 1, 2, 3 };
	int cycle, k;

	pc_begin("XSH.hcreate.004");
	pc_test();
	for (cycle = 0; cycle < 3; cycle++) {
		ENTRY e, *r;

		pc_check(hcreate(8) != 0, "hcreate() failed in cycle %d", cycle);
		for (k = 0; k < 3; k++) {
			e.key = keys[k];
			e.data = NULL;
			pc_check(hsearch(e, FIND) == NULL, "cycle %d: %s is found in a new table", cycle, keys[k]);
		}
		for (k = 0; k < 3; k++) {
			e.key = keys[k];
			e.data = &data[k];
			pc_check(hsearch(e, ENTER) != NULL, "cycle %d: ENTER of %s failed", cycle, keys[k]);
		}
		for (k = 0; k < 3; k++) {
			e.key = keys[k];
			e.data = NULL;
			r = hsearch(e, FIND);
			pc_check(r != NULL && r->data == &data[k], "cycle %d: %s was not found", cycle, keys[k]);
		}
		hdestroy();
	}
	pc_pass();
}
