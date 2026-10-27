/*
 * sem_post() releases a thread blocked in sem_wait(), and the post is
 * consumed by it: the value stays 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_post.002
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct job j;
	int r;

	pc_begin("XSH.sem_post.002");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	job_start(&j, do_wait, &s);
	sleep_ms(200);
	pc_check(!atomic_load(&j.done), "sem_wait() returned (%d) on a value 0 semaphore before any post", j.result);
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d (%s)", r, pc_errname(errno));
	job_finish(&j, "sem_wait() after a post");
	pc_check(j.result == 0, "sem_wait() returned %d (%s), expected 0", j.result, rn(j.err));
	expect_count(&s, 0, "after the waiter took the post");
	pc_pass();
}
