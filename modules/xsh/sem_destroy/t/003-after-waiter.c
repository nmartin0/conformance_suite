/*
 * A semaphore that a thread has waited on (and been released from) can
 * be destroyed once no thread is blocked on it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_destroy.003
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct job j;
	int r;

	pc_begin("XSH.sem_destroy.003");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	job_start(&j, do_wait, &s);
	sleep_ms(100);
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d", r);
	job_finish(&j, "sem_wait() after a post");
	pc_check(j.result == 0, "sem_wait() returned %d", j.result);
	r = sem_destroy(&s);
	pc_check(r == 0, "sem_destroy() of a semaphore with no blocked thread returned %d (%s), expected 0", r,
	    r == -1 ? pc_errname(errno) : "-");
	pc_pass();
}
