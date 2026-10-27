/*
 * sem_trywait() on a locked semaphore returns -1 with EAGAIN at once and
 * leaves the semaphore as it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_wait.005
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	int r, i;

	pc_begin("XSH.sem_wait.005");
	guard(30);
	new_sem(&s, 0, 0);
	pc_test();
	for (i = 0; i < 3; i++) {
		errno = 0;
		r = sem_trywait(&s);
		pc_check(r == -1, "sem_trywait() on a value 0 semaphore returned %d, expected -1", r);
		pc_check(errno == EAGAIN, "errno is %s, expected EAGAIN", pc_errname(errno));
	}
	/* Had the failed calls lowered the value, one post would not give a success. */
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d", r);
	expect_count(&s, 1, "after three failed sem_trywait() calls and one post");
	pc_pass();
}
