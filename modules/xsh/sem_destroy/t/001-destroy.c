/*
 * sem_destroy() of an initialized semaphore nobody waits on returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_destroy.001
 */
#include "semh.h"

int main(void)
{
	static const unsigned vals[] = { 0, 3 };
	sem_t s;
	size_t i;
	int r;

	pc_begin("XSH.sem_destroy.001");
	guard(30);
	pc_test();
	for (i = 0; i < 2; i++) {
		new_sem(&s, 0, vals[i]);
		r = sem_destroy(&s);
		pc_check(r == 0, "sem_destroy() of a value %u semaphore returned %d (%s), expected 0", vals[i], r,
		    r == -1 ? pc_errname(errno) : "-");
	}
	pc_pass();
}
