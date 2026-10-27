/*
 * After sem_unlink() the name can be created again, and the new semaphore
 * is a different one from the one still open under the old name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.003
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;

	pc_begin("XSH.sem_unlink.003");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 3);
	pc_test();
	pc_check(sem_unlink(n) == 0, "sem_unlink() failed (%s)", pc_errname(errno));
	b = sem_open(n, O_CREAT | O_EXCL, (mode_t)0600, 1u);
	pc_check(b != SEM_FAILED, "sem_open(O_CREAT|O_EXCL) of the unlinked name failed (%s)", pc_errname(errno));
	pc_check(a != b, "the new semaphore has the handle of the old one still open");
	expect_count(b, 1, "the semaphore created again under the name");
	expect_count(a, 3, "the old semaphore, still open");
	(void)sem_close(a);
	(void)sem_close(b);
	pc_pass();
}
