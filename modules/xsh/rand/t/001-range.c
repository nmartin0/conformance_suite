/*
 * rand() values lie in [0, RAND_MAX].
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rand.001
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	int i, first, differ = 0;

	pc_begin("XSH.rand.001");
	pc_test();
	pc_check(RAND_MAX >= 32767, "RAND_MAX is %d, below 32767", RAND_MAX);
	srand(1);
	first = rand();
	for (i = 0; i < 1000; i++) {
		int r = i == 0 ? first : rand();

		pc_check(r >= 0 && r <= RAND_MAX, "rand() returned %d, outside [0, RAND_MAX]", r);
		if (r != first)
			differ = 1;
	}
	pc_check(differ, "1000 consecutive rand() values were all equal");
	pc_pass();
}
