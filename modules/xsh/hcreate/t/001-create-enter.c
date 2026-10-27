/*
 * hcreate() and hsearch(ENTER).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.hcreate.001
 */
#include "pc.h"
#include <search.h>
#include <stdio.h>
#include <string.h>

#define N 50

int main(void)
{
	static char keys[N][16];
	static int data[N];
	int i;

	pc_begin("XSH.hcreate.001");
	for (i = 0; i < N; i++) {
		(void)snprintf(keys[i], sizeof keys[i], "key-%d", i);
		data[i] = i * 11;
	}
	pc_test();
	pc_check(hcreate(N) != 0, "hcreate(%d) failed", N);
	for (i = 0; i < N; i++) {
		ENTRY item, *r;

		item.key = keys[i];
		item.data = &data[i];
		r = hsearch(item, ENTER);
		pc_check(r != NULL, "hsearch(ENTER) of %s returned a null pointer", keys[i]);
		pc_check(r->key != NULL && strcmp(r->key, keys[i]) == 0, "the entry for %s has key %s", keys[i], r->key ? r->key : "(null)");
		pc_check(r->data == &data[i], "the entry for %s has the wrong data pointer", keys[i]);
	}
	hdestroy();
	pc_pass();
}
