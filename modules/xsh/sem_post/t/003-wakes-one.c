/*
 * One sem_post() releases exactly one of two blocked threads; a second
 * releases the other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_post.003
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct job j[2];
	int r, n;

	pc_begin("XSH.sem_post.003");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	job_start(&j[0], do_wait, &s);
	job_start(&j[1], do_wait, &s);
	sleep_ms(300);
	pc_check(!atomic_load(&j[0].done) && !atomic_load(&j[1].done),
	    "a sem_wait() returned on a value 0 semaphore before any post");
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d", r);
	/* Wait (10 s at most) for one thread to return, then give another time to. */
	for (n = 0; n < 2000 && !atomic_load(&j[0].done) && !atomic_load(&j[1].done); n++)
		sleep_ms(5);
	sleep_ms(300);
	n = atomic_load(&j[0].done) + atomic_load(&j[1].done);
	pc_check(n == 1, "after one post, %d of the two blocked threads returned, expected exactly 1", n);
	r = sem_post(&s);
	pc_check(r == 0, "second sem_post() returned %d", r);
	job_finish(&j[0], "the first waiter");
	job_finish(&j[1], "the second waiter");
	pc_check(j[0].result == 0 && j[1].result == 0, "the waiters returned %d and %d, expected 0", j[0].result,
	    j[1].result);
	pc_pass();
}
