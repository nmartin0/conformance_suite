/*
 * sem_destroy() leaves no descriptor open: the lowest free descriptor is
 * the same after repeated sem_init()/sem_destroy() cycles as before them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_destroy.004
 */
#include "semh.h"

static void cycle(int pshared)
{
	sem_t s;

	new_sem(&s, pshared, 1);
	NEED(sem_destroy(&s) == 0, "sem_destroy failed: %s", pc_errname(errno));
}

int main(void)
{
	int before, after, i;

	pc_begin("XSH.sem_destroy.004");
	guard(30);
	/* A first cycle of each kind, in case the system sets up something once. */
	cycle(0);
	cycle(1);
	before = pc_lowest_free_fd();
	pc_test();
	for (i = 0; i < 20; i++)
		cycle(i & 1);
	after = pc_lowest_free_fd();
	pc_check(after == before, "the lowest free descriptor was %d before and is %d after 20 init/destroy cycles",
	    before, after);
	pc_pass();
}
