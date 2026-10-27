/*
 * random() by default matches initstate(1, state, 128).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.003
 */
#include "pc.h"
#include <stdlib.h>

static long state[256 / sizeof(long) + 1];

int main(void)
{
	long a[50], b[50];
	int i;

	pc_begin("XSH.random.003");
	pc_test();
	for (i = 0; i < 50; i++)
		a[i] = random();
	pc_check(initstate(1, (char *)state, 128) != NULL, "initstate(1, state, 128) failed");
	for (i = 0; i < 50; i++)
		b[i] = random();
	for (i = 0; i < 50; i++)
		pc_check(a[i] == b[i], "value %d: %ld by default, %ld after initstate(1, state, 128)", i, a[i], b[i]);
	pc_pass();
}
