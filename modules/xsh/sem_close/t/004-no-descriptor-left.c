/*
 * sem_close() releases what sem_open() took: the lowest free descriptor
 * is the same after repeated open/close cycles as before them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_close.004
 */
#include "semh.h"

static void cycle(const char *n)
{
	sem_t *s = sem_open(n, 0);

	NEED(s != SEM_FAILED, "sem_open failed: %s", pc_errname(errno));
	NEED(sem_close(s) == 0, "sem_close failed: %s", pc_errname(errno));
}

int main(void)
{
	const char *n;
	sem_t *a;
	int before, after, i;

	pc_begin("XSH.sem_close.004");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 1);
	/* Keep one handle open throughout; cycle once to let the system set up. */
	cycle(n);
	before = pc_lowest_free_fd();
	pc_test();
	for (i = 0; i < 20; i++)
		cycle(n);
	after = pc_lowest_free_fd();
	pc_check(after == before, "the lowest free descriptor was %d before and is %d after 20 open/close cycles",
	    before, after);
	(void)sem_close(a);
	pc_pass();
}
