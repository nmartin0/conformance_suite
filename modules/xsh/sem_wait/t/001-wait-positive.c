/*
 * sem_wait() on a positive value returns 0 at once and lowers the value
 * by exactly one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_wait.001
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	int r;

	pc_begin("XSH.sem_wait.001");
	guard(30);
	new_sem(&s, 0, 3);
	pc_test();
	r = sem_wait(&s);
	pc_check(r == 0, "sem_wait() on a value 3 semaphore returned %d (%s)", r, pc_errname(errno));
	expect_count(&s, 2, "after one sem_wait() of value 3");
	pc_check(sem_destroy(&s) == 0, "sem_destroy failed");
	new_sem(&s, 0, 1);
	r = sem_wait(&s);
	pc_check(r == 0, "sem_wait() on a value 1 semaphore returned %d (%s)", r, pc_errname(errno));
	expect_count(&s, 0, "after sem_wait() of value 1");
	pc_pass();
}
