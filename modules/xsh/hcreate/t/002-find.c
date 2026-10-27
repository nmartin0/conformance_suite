/*
 * hsearch(FIND).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.hcreate.002
 */
#include "pc.h"
#include <search.h>
#include <stdio.h>
#include <string.h>

#define N 30

int main(void)
{
	static char keys[N][16];
	static int data[N];
	int i;

	pc_begin("XSH.hcreate.002");
	for (i = 0; i < N; i++) {
		(void)snprintf(keys[i], sizeof keys[i], "name%d", i);
		data[i] = i + 100;
	}
	pc_check(hcreate(N) != 0, "setup: hcreate() failed");
	for (i = 0; i < N; i++) {
		ENTRY item;

		item.key = keys[i];
		item.data = &data[i];
		pc_check(hsearch(item, ENTER) != NULL, "setup: hsearch(ENTER) failed");
	}
	pc_test();
	for (i = 0; i < N; i++) {
		char other[16];
		ENTRY q, *r;

		(void)snprintf(other, sizeof other, "name%d", i);	/* same text, different buffer */
		q.key = other;
		q.data = NULL;
		r = hsearch(q, FIND);
		pc_check(r != NULL, "hsearch(FIND) did not find %s", other);
		pc_check(strcmp(r->key, other) == 0, "the entry found for %s has key %s", other, r->key);
		pc_check(r->data == &data[i], "the entry found for %s has the wrong data", other);
	}
	{
		ENTRY q;

		q.key = (char *)"name";
		q.data = NULL;
		pc_check(hsearch(q, FIND) == NULL, "a prefix of an entered key was found");
		q.key = (char *)"absent";
		pc_check(hsearch(q, FIND) == NULL, "an absent key was found");
		pc_check(hsearch(q, FIND) == NULL, "an absent key was found on the second lookup (FIND entered it)");
	}
	hdestroy();
	pc_pass();
}
