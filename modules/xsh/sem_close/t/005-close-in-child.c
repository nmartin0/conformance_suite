/*
 * A process that opens a semaphore, posts and closes it does not take the
 * semaphore away from another process: the parent's handle still works
 * and sees the post.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_close.005
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a;
	pid_t k;
	int r;

	pc_begin("XSH.sem_close.005");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 0);
	pc_test();
	k = fork();
	NEED(k >= 0, "fork failed");
	if (k == 0) {
		sem_t *s = sem_open(n, 0);

		if (s == SEM_FAILED || sem_post(s) != 0)
			_exit(10);
		_exit(sem_close(s) == 0 ? 0 : 11);
	}
	r = reap(k);
	pc_check(r == 0, "the child's sem_open, sem_post or sem_close failed (status %d)", r);
	expect_count(a, 1, "in the parent after the child's post and close");
	pc_check(sem_post(a) == 0, "sem_post() through the parent's handle failed");
	expect_count(a, 1, "in the parent after its own post");
	pc_check(sem_close(a) == 0, "sem_close() in the parent failed");
	pc_pass();
}
