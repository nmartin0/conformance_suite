/*
 * A sem_wait() hit by a signal either fails with EINTR, leaving the
 * semaphore as it was, or goes on waiting until a post.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_wait.003
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	struct job j;
	int r;

	pc_begin("XSH.sem_wait.003");
	guard(40);
	new_sem(&s, 0, 0);
	catch_usr1();
	pc_test();
	job_start(&j, do_wait, &s);
	sleep_ms(100);
	signal_job(&j);
	sleep_ms(300);
	if (atomic_load(&j.done)) {
		/* Interrupted: the only permitted outcome of a return is EINTR. */
		pc_check(j.result == -1 && j.err == EINTR,
		    "sem_wait() returned %d (%s) after a signal with no post, expected -1 with EINTR or still waiting",
		    j.result, rn(j.err));
		NEED(pthread_join(j.thread, NULL) == 0, "pthread_join failed");
		expect_count(&s, 0, "after an interrupted sem_wait()");
		r = sem_post(&s);
		pc_check(r == 0, "sem_post() returned %d", r);
		expect_count(&s, 1, "after one post");
	} else {
		r = sem_post(&s);
		pc_check(r == 0, "sem_post() returned %d", r);
		job_finish(&j, "sem_wait() after a post");
		pc_check(j.result == 0, "sem_wait() returned %d (%s) after the post", j.result, rn(j.err));
		expect_count(&s, 0, "after the wait took the post");
	}
	pc_pass();
}
