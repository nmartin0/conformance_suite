/*
 * Successive clock() values never decrease.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock.002
 */
#include "pc.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	clock_t prev, c;
	int i;

	pc_begin("XSH.clock.002");
	alarm(60);
	pc_test();
	prev = clock();
	if (prev == (clock_t)-1)
		pc_notinuse("the processor time is reported as not available");
	for (i = 0; i < 200000; i++) {
		c = clock();
		pc_check(c != (clock_t)-1, "clock() failed after succeeding (errno %d)", errno);
		pc_check(c >= prev, "clock() went back from %ld to %ld", (long)prev, (long)c);
		prev = c;
	}
	pc_pass();
}
