/*
 * A second sem_open() of the name, without O_CREAT, gives access to the
 * same semaphore (a post by one handle is seen through the other).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.004
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;
	int v;

	pc_begin("XSH.sem_open.004");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 0);
	pc_test();
	b = sem_open(n, 0);
	pc_check(b != SEM_FAILED, "sem_open() of an existing name returned SEM_FAILED (%s)", pc_errname(errno));
	pc_check(sem_post(b) == 0, "sem_post() through the second handle failed");
	pc_check(sem_trywait(a) == 0, "sem_trywait() through the first handle did not see the post (%s)",
	    pc_errname(errno));
	pc_check(sem_post(a) == 0, "sem_post() failed");
	pc_check(sem_post(a) == 0, "sem_post() failed");
	v = -77;
	pc_check(sem_getvalue(b, &v) == 0, "sem_getvalue() failed");
	pc_check(v == 2, "through the second handle the value is %d, expected 2", v);
	expect_count(b, 2, "through the second handle");
	pc_check(sem_close(a) == 0, "sem_close failed");
	if (b != a)
		pc_check(sem_close(b) == 0, "sem_close failed");
	pc_pass();
}
