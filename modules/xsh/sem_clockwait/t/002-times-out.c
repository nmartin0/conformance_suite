/*
 * On a semaphore of value 0, a timed wait fails with ETIMEDOUT, and not
 * before its clock has reached the absolute time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.002
 */
#include "semh.h"

int main(void)
{
	struct timespec abs, after;
	sem_t s;
	size_t i;
	int r;

	pc_begin("XSH.sem_clockwait.002");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	for (i = 0; i < 3; i++) {
		abs = deadline(&wfs[i], 200);
		errno = 0;
		r = do_timed(&wfs[i], &s, &abs);
		clk_now(wfs[i].clk, &after);
		pc_check(r == -1, "%s on a value 0 semaphore returned %d, expected -1", wfs[i].name, r);
		pc_check(errno == ETIMEDOUT, "%s: errno is %s, expected ETIMEDOUT", wfs[i].name, pc_errname(errno));
		pc_check(ts_ge(&after, &abs), "%s returned before the clock reached the absolute time", wfs[i].name);
	}
	pc_pass();
}
