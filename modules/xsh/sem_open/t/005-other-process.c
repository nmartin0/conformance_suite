/*
 * A named semaphore is shared with another process that opens the same
 * name: posts go both ways.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.005
 */
#include "semh.h"

int main(void)
{
	const char *na, *nb;
	struct timespec t;
	sem_t *a, *b;
	pid_t k;
	int r;

	pc_begin("XSH.sem_open.005");
	guard(50);
	na = sn_make("a");
	nb = sn_make("b");
	a = sn_create(na, 0);
	b = sn_create(nb, 0);
	pc_test();
	k = fork();
	NEED(k >= 0, "fork failed");
	if (k == 0) {
		/* The child posts a, then waits for the parent's post of b. */
		sem_t *ca = sem_open(na, 0), *cb = sem_open(nb, 0);

		if (ca == SEM_FAILED || cb == SEM_FAILED)
			_exit(10);
		if (sem_post(ca) != 0)
			_exit(11);
		clk_now(CLOCK_REALTIME, &t);
		ts_add_ms(&t, 20000);
		if (sem_timedwait(cb, &t) != 0)
			_exit(12);
		_exit(0);
	}
	clk_now(CLOCK_REALTIME, &t);
	ts_add_ms(&t, 20000);
	r = sem_timedwait(a, &t);
	if (r != 0) {
		int e = errno;

		(void)kill(k, SIGKILL);
		(void)reap(k);
		pc_check(0, "the child's post was not seen by the parent: %s", pc_errname(e));
	}
	r = sem_post(b);
	pc_check(r == 0, "sem_post() returned %d", r);
	r = reap(k);
	pc_check(r == 0, "the child failed (status %d: 10 open, 11 post, 12 wait for the parent's post)", r);
	pc_pass();
}
