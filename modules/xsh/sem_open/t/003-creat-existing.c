/*
 * sem_open() with O_CREAT alone on an existing name connects to it and
 * the value argument has no effect.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.003
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;

	pc_begin("XSH.sem_open.003");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 2);
	pc_test();
	b = sem_open(n, O_CREAT, (mode_t)0600, 9u);
	pc_check(b != SEM_FAILED, "sem_open(O_CREAT) of an existing name returned SEM_FAILED (%s)", pc_errname(errno));
	expect_count(b, 2, "through the second handle, the value given at creation");
	pc_check(sem_post(a) == 0, "sem_post failed");
	expect_count(b, 1, "one post through the first handle, counted through the second");
	pc_check(sem_close(a) == 0, "sem_close failed");
	if (b != a)
		pc_check(sem_close(b) == 0, "sem_close failed");
	pc_pass();
}
