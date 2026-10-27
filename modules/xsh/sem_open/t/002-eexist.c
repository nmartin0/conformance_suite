/*
 * sem_open() with O_CREAT|O_EXCL on an existing name fails with EEXIST
 * and does not change the existing semaphore.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.002
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;

	pc_begin("XSH.sem_open.002");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 2);
	pc_test();
	errno = 0;
	b = sem_open(n, O_CREAT | O_EXCL, (mode_t)0600, 7u);
	if (b != SEM_FAILED)
		pc_check(0, "sem_open(O_CREAT|O_EXCL) of an existing name succeeded");
	pc_check(errno == EEXIST, "errno is %s, expected EEXIST", pc_errname(errno));
	expect_count(a, 2, "the existing semaphore after the failed exclusive create");
	pc_check(sem_close(a) == 0, "sem_close failed");
	pc_pass();
}
