/*
 * random() by default matches srandom(1).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.002
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	long a[50], b[50];
	int i;

	pc_begin("XSH.random.002");
	pc_test();
	for (i = 0; i < 50; i++)
		a[i] = random();
	srandom(1);
	for (i = 0; i < 50; i++)
		b[i] = random();
	for (i = 0; i < 50; i++)
		pc_check(a[i] >= 0 && a[i] <= 2147483647L && b[i] >= 0 && b[i] <= 2147483647L,
		    "value %d is outside 0 to 2^31-1: %ld by default, %ld after srandom(1)", i, a[i], b[i]);
	for (i = 0; i < 50; i++)
		pc_check(a[i] == b[i], "value %d: %ld by default, %ld after srandom(1)", i, a[i], b[i]);
	pc_pass();
}
