/*
 * sem_unlink() returns 0 for an existing name; the name then cannot be
 * opened and a second sem_unlink() fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.001
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;
	int r;

	pc_begin("XSH.sem_unlink.001");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 1);
	pc_test();
	r = sem_unlink(n);
	pc_check(r == 0, "sem_unlink() of an existing name returned %d (%s), expected 0", r,
	    r == -1 ? pc_errname(errno) : "-");
	errno = 0;
	b = sem_open(n, 0);
	if (b != SEM_FAILED)
		pc_check(0, "sem_open() of the unlinked name succeeded");
	pc_check(errno == ENOENT, "sem_open() of the unlinked name: errno is %s, expected ENOENT", pc_errname(errno));
	errno = 0;
	r = sem_unlink(n);
	pc_check(r == -1, "a second sem_unlink() returned %d, expected -1", r);
	pc_check(errno == ENOENT, "a second sem_unlink(): errno is %s, expected ENOENT", pc_errname(errno));
	(void)sem_close(a);
	pc_pass();
}
