/*
 * sem_init() gives the semaphore the initial value asked for.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_init.001
 */
#include "semh.h"

int main(void)
{
	static const unsigned vals[] = { 0, 1, 3 };
	sem_t s;
	size_t i;
	int r;

	pc_begin("XSH.sem_init.001");
	guard(30);
	pc_test();
	for (i = 0; i < sizeof vals / sizeof vals[0]; i++) {
		r = sem_init(&s, 0, vals[i]);
		pc_check(r == 0, "sem_init(value %u) returned %d (errno %s), expected 0", vals[i], r,
		    r == -1 ? pc_errname(errno) : "-");
		expect_count(&s, (int)vals[i], "after sem_init");
		NEED(sem_destroy(&s) == 0, "sem_destroy failed");
	}
	pc_pass();
}
