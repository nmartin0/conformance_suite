/*
 * With pshared 0 the semaphore is shared between the threads of the
 * process: a post in one thread releases a wait in another, both ways.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_init.003
 */
#include "semh.h"

static sem_t a, b;

static int ping(void *p)
{
	(void)p;
	if (sem_wait(&a) != 0)
		return 1;
	return sem_post(&b) == 0 ? 0 : 2;
}

int main(void)
{
	struct job j;
	int r;

	pc_begin("XSH.sem_init.003");
	guard(60);
	new_sem(&a, 0, 0);
	new_sem(&b, 0, 0);
	pc_test();
	/* The other thread waits on a and then posts b. */
	job_start(&j, ping, NULL);
	sleep_ms(100);
	pc_check(!atomic_load(&j.done), "sem_wait() on a value 0 semaphore returned before any post");
	r = sem_post(&a);
	pc_check(r == 0, "sem_post() returned %d", r);
	r = sem_wait(&b);
	pc_check(r == 0, "sem_wait() for the other thread's post returned %d (%s)", r, pc_errname(errno));
	job_finish(&j, "the helper thread");
	pc_check(j.result == 0, "the helper thread's operations failed (%d)", j.result);
	pc_pass();
}
