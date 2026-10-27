/*
 * Time spent sleeping is not processor time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock.004
 */
#include "pc.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	struct timespec req, rem;
	clock_t c0, c1;
	double cpu;
	int r;

	pc_begin("XSH.clock.004");
	alarm(30);
	pc_test();
	c0 = clock();
	if (c0 == (clock_t)-1)
		pc_notinuse("the processor time is reported as not available");
	req.tv_sec = 0;
	req.tv_nsec = 400000000L;
	while ((r = nanosleep(&req, &rem)) == -1 && errno == EINTR)
		req = rem;
	pc_check(r == 0, "nanosleep failed");
	c1 = clock();
	pc_check(c1 != (clock_t)-1, "clock() failed after succeeding");
	cpu = (double)(c1 - c0) / (double)CLOCKS_PER_SEC;
	pc_check(cpu < 0.2, "clock() advanced %f s over a 0.4 s sleep", cpu);
	pc_pass();
}
