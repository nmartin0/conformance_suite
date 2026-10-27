/*
 * An initialized semaphore can be used with every operation of the page
 * until it is destroyed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_init.002
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct timespec t;
	int r, i;

	pc_begin("XSH.sem_init.002");
	guard(30);
	pc_test();
	r = sem_init(&s, 0, 1);
	pc_check(r == 0, "sem_init returned %d", r);
	r = sem_wait(&s);
	pc_check(r == 0, "sem_wait() on a value 1 semaphore returned %d (%s)", r, pc_errname(errno));
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d (%s)", r, pc_errname(errno));
	r = sem_trywait(&s);
	pc_check(r == 0, "sem_trywait() returned %d (%s)", r, pc_errname(errno));
	for (i = 0; i < 3; i++) {
		r = sem_post(&s);
		pc_check(r == 0, "sem_post() returned %d (%s)", r, pc_errname(errno));
		clk_now(CLOCK_REALTIME, &t);
		ts_add_ms(&t, 20000);
		if (i == 0)
			r = sem_timedwait(&s, &t);
		else if (i == 1)
			r = sem_clockwait(&s, CLOCK_REALTIME, &t);
		else {
			clk_now(CLOCK_MONOTONIC, &t);
			ts_add_ms(&t, 20000);
			r = sem_clockwait(&s, CLOCK_MONOTONIC, &t);
		}
		pc_check(r == 0, "timed wait %d returned %d (%s)", i, r, pc_errname(errno));
	}
	expect_count(&s, 0, "after all operations balanced");
	r = sem_destroy(&s);
	pc_check(r == 0, "sem_destroy returned %d (%s)", r, pc_errname(errno));
	pc_pass();
}
