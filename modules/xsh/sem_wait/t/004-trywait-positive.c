/*
 * sem_trywait() on a positive value returns 0 and lowers it by one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_wait.004
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	int r;

	pc_begin("XSH.sem_wait.004");
	guard(30);
	new_sem(&s, 0, 3);
	pc_test();
	r = sem_trywait(&s);
	pc_check(r == 0, "sem_trywait() on a value 3 semaphore returned %d (%s)", r, pc_errname(errno));
	r = sem_trywait(&s);
	pc_check(r == 0, "sem_trywait() on a value 2 semaphore returned %d (%s)", r, pc_errname(errno));
	expect_count(&s, 1, "after two sem_trywait() calls of value 3");
	pc_pass();
}
