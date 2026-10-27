/*
 * sem_open() without O_CREAT of a name that does not exist fails with
 * ENOENT and returns SEM_FAILED.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.006
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a;

	pc_begin("XSH.sem_open.006");
	guard(30);
	n = sn_make("a");
	pc_test();
	errno = 0;
	a = sem_open(n, 0);
	if (a != SEM_FAILED)
		pc_check(0, "sem_open() of a name that does not exist succeeded");
	pc_check(errno == ENOENT, "errno is %s, expected ENOENT", pc_errname(errno));
	pc_pass();
}
