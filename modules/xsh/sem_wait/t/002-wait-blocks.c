/*
 * sem_wait() on a semaphore of value 0 does not return until a post.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_wait.002
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct job j;
	int r;

	pc_begin("XSH.sem_wait.002");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	job_start(&j, do_wait, &s);
	sleep_ms(300);
	pc_check(!atomic_load(&j.done), "sem_wait() returned %d (%s) on a value 0 semaphore with no post", j.result,
	    rn(j.err));
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d", r);
	job_finish(&j, "sem_wait() after a post");
	pc_check(j.result == 0, "sem_wait() returned %d (%s) after the post, expected 0", j.result, rn(j.err));
	expect_count(&s, 0, "after the wait took the post");
	pc_pass();
}
