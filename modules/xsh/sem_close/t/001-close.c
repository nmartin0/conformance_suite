/*
 * sem_close() of the handle sem_open() returned gives 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_close.001
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a;
	int r;

	pc_begin("XSH.sem_close.001");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 1);
	pc_test();
	r = sem_close(a);
	pc_check(r == 0, "sem_close() returned %d (%s), expected 0", r, r == -1 ? pc_errname(errno) : "-");
	pc_pass();
}
