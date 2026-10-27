/*
 * rand() without srand() acts as if seeded with 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rand.003
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	int a[50], b[50], i;

	pc_begin("XSH.rand.003");
	pc_test();
	for (i = 0; i < 50; i++)
		a[i] = rand();
	srand(1);
	for (i = 0; i < 50; i++)
		b[i] = rand();
	for (i = 0; i < 50; i++)
		pc_check(a[i] == b[i], "value %d differs: %d without srand(), %d after srand(1)", i, a[i], b[i]);
	pc_pass();
}
