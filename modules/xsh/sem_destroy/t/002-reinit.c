/*
 * After sem_destroy(), sem_init() on the same object gives a usable
 * semaphore with the new value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_destroy.002
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	int r;

	pc_begin("XSH.sem_destroy.002");
	guard(30);
	new_sem(&s, 0, 5);
	pc_test();
	r = sem_destroy(&s);
	pc_check(r == 0, "sem_destroy() returned %d", r);
	r = sem_init(&s, 0, 2);
	pc_check(r == 0, "sem_init() of the destroyed semaphore returned %d (%s)", r, pc_errname(errno));
	expect_count(&s, 2, "after destroy and sem_init(2)");
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() on the reinitialized semaphore returned %d", r);
	expect_count(&s, 1, "after a post");
	r = sem_destroy(&s);
	pc_check(r == 0, "second sem_destroy() returned %d", r);
	pc_pass();
}
