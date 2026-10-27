/*
 * Once unlinked and closed, a semaphore is gone: the name made again has
 * a fresh value, not the old one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.005
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;

	pc_begin("XSH.sem_unlink.005");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 3);
	pc_test();
	pc_check(sem_unlink(n) == 0, "sem_unlink() failed (%s)", pc_errname(errno));
	pc_check(sem_close(a) == 0, "sem_close() of the unlinked semaphore failed (%s)", pc_errname(errno));
	b = sem_open(n, O_CREAT | O_EXCL, (mode_t)0600, 1u);
	pc_check(b != SEM_FAILED, "sem_open(O_CREAT|O_EXCL) after unlink and close failed (%s)", pc_errname(errno));
	expect_count(b, 1, "the semaphore created again after unlink and close");
	(void)sem_close(b);
	pc_pass();
}
