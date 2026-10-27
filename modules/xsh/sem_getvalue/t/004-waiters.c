/*
 * With threads blocked on the semaphore, sem_getvalue() gives zero or the
 * negative of a number of waiters, and does not disturb them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_getvalue.004
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct job j[2];
	int r, v;

	pc_begin("XSH.sem_getvalue.004");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	job_start(&j[0], do_wait, &s);
	job_start(&j[1], do_wait, &s);
	sleep_ms(300);
	v = 12345;
	r = sem_getvalue(&s, &v);
	pc_check(r == 0, "sem_getvalue() returned %d (%s)", r, pc_errname(errno));
	pc_check(v <= 0 && v >= -2, "with two threads waiting, sem_getvalue() gave %d, expected 0, -1 or -2", v);
	pc_check(!atomic_load(&j[0].done) && !atomic_load(&j[1].done), "a waiter returned without a post");
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d", r);
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d", r);
	job_finish(&j[0], "the first waiter");
	job_finish(&j[1], "the second waiter");
	pc_check(j[0].result == 0 && j[1].result == 0, "a waiter returned %d and %d, expected 0", j[0].result,
	    j[1].result);
	v = 12345;
	r = sem_getvalue(&s, &v);
	pc_check(r == 0 && v == 0, "after both waiters were released, sem_getvalue() gave %d (returned %d), expected 0", v, r);
	pc_pass();
}
