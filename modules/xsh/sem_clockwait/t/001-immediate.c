/*
 * A timed wait that can lock the semaphore at once does so and returns 0,
 * even when the timeout has passed already.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.001
 */
#include "semh.h"

int main(void)
{
	struct timespec past = { 0, 0 };
	sem_t s;
	size_t i;
	int r;

	pc_begin("XSH.sem_clockwait.001");
	guard(40);
	pc_test();
	for (i = 0; i < 3; i++) {
		new_sem(&s, 0, 2);
		r = do_timed(&wfs[i], &s, &past);
		pc_check(r == 0, "%s on a value 2 semaphore with an expired timeout returned %d (%s), expected 0",
		    wfs[i].name, r, pc_errname(errno));
		expect_count(&s, 1, wfs[i].name);
		NEED(sem_destroy(&s) == 0, "sem_destroy failed");
	}
	pc_pass();
}
