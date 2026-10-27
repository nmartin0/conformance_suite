/*
 * sem_open() with O_CREAT|O_EXCL creates a semaphore with the given
 * initial value; a handle is not SEM_FAILED; two names are two semaphores.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.001
 */
#include "semh.h"

int main(void)
{
	const char *na, *nb;
	sem_t *a, *b;

	pc_begin("XSH.sem_open.001");
	guard(30);
	na = sn_make("a");
	nb = sn_make("b");
	pc_test();
	a = sem_open(na, O_CREAT | O_EXCL, (mode_t)0600, 0u);
	pc_check(a != SEM_FAILED, "sem_open(O_CREAT|O_EXCL, value 0) returned SEM_FAILED (%s)", pc_errname(errno));
	b = sem_open(nb, O_CREAT | O_EXCL, (mode_t)0600, 2u);
	pc_check(b != SEM_FAILED, "sem_open(O_CREAT|O_EXCL, value 2) returned SEM_FAILED (%s)", pc_errname(errno));
	expect_count(b, 2, "a semaphore created with value 2");
	pc_check(sem_post(a) == 0, "sem_post() returned -1");
	/* b is locked now: the post to the other name must not show there. */
	expect_count(b, 0, "the other semaphore after a post to this one");
	expect_count(a, 1, "a semaphore created with value 0 and posted once");
	pc_check(sem_close(a) == 0, "sem_close(a) failed");
	pc_check(sem_close(b) == 0, "sem_close(b) failed");
	pc_pass();
}
