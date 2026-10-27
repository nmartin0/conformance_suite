/*
 * A handle opened before sem_unlink() goes on working, with its state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.004
 */
#include "semh.h"

int main(void)
{
	const char *n;
	struct timespec t;
	sem_t *a;
	int r;

	pc_begin("XSH.sem_unlink.004");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 2);
	pc_test();
	pc_check(sem_unlink(n) == 0, "sem_unlink() failed (%s)", pc_errname(errno));
	r = sem_wait(a);
	pc_check(r == 0, "sem_wait() on the handle after sem_unlink() returned %d (%s)", r, pc_errname(errno));
	pc_check(sem_post(a) == 0, "sem_post() on the handle after sem_unlink() failed");
	pc_check(sem_post(a) == 0, "sem_post() on the handle after sem_unlink() failed");
	clk_now(CLOCK_REALTIME, &t);
	ts_add_ms(&t, 20000);
	r = sem_timedwait(a, &t);
	pc_check(r == 0, "sem_timedwait() on the handle after sem_unlink() returned %d (%s)", r, pc_errname(errno));
	expect_count(a, 2, "on the handle after sem_unlink()");
	pc_check(sem_close(a) == 0, "sem_close() of an unlinked semaphore failed (%s)", pc_errname(errno));
	pc_pass();
}
