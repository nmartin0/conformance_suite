/*
 * clock() advances while the process computes and does not outrun elapsed real time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock.003
 */
#include "pc.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>

static double elapsed(const struct timespec *a, const struct timespec *b)
{
	return (double)(b->tv_sec - a->tv_sec) + (double)(b->tv_nsec - a->tv_nsec) / 1e9;
}

int main(void)
{
	struct timespec w0, w1;
	clock_t c0, c1;
	volatile unsigned long x = 0;
	double cpu, wall;

	pc_begin("XSH.clock.003");
	alarm(90);
	pc_test();
	c0 = clock();
	if (c0 == (clock_t)-1)
		pc_notinuse("the processor time is reported as not available");
	pc_check(clock_gettime(CLOCK_MONOTONIC, &w0) == 0, "clock_gettime failed");
	do {
		unsigned long k;

		for (k = 0; k < 100000; k++)
			x += k;
		c1 = clock();
		pc_check(c1 != (clock_t)-1, "clock() failed after succeeding");
		pc_check(clock_gettime(CLOCK_MONOTONIC, &w1) == 0, "clock_gettime failed");
	} while ((double)(c1 - c0) / (double)CLOCKS_PER_SEC < 0.05 && elapsed(&w0, &w1) < 30.0);
	cpu = (double)(c1 - c0) / (double)CLOCKS_PER_SEC;
	wall = elapsed(&w0, &w1);
	pc_check(cpu >= 0.05, "clock() advanced only %f s in %f s of computing", cpu, wall);
	pc_check(cpu <= wall + 0.1, "clock() advanced %f s in only %f s of real time", cpu, wall);
	pc_pass();
}
