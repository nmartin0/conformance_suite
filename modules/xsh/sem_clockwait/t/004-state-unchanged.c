/*
 * A timed wait that timed out leaves the semaphore unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.004
 */
#include "semh.h"

int main(void)
{
	struct timespec abs;
	sem_t s;
	size_t i;
	int r;

	pc_begin("XSH.sem_clockwait.004");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	for (i = 0; i < 3; i++) {
		abs = deadline(&wfs[i], 50);
		r = do_timed(&wfs[i], &s, &abs);
		pc_check(r == -1 && errno == ETIMEDOUT, "%s did not time out (returned %d, %s)", wfs[i].name, r,
		    pc_errname(errno));
		expect_count(&s, 0, "after a timeout");
		r = sem_post(&s);
		pc_check(r == 0, "sem_post() returned %d", r);
		expect_count(&s, 1, "after a timeout and one post");
	}
	pc_pass();
}
