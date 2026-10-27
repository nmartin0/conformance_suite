/*
 * A semaphore that has not been unlinked keeps its state after sem_close():
 * the name opens again and the value is what it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_close.002
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;
	int v;

	pc_begin("XSH.sem_close.002");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 0);
	NEED(sem_post(a) == 0, "sem_post failed");
	NEED(sem_post(a) == 0, "sem_post failed");
	pc_test();
	pc_check(sem_close(a) == 0, "sem_close() failed");
	b = sem_open(n, 0);
	pc_check(b != SEM_FAILED, "sem_open() of the name after sem_close() failed (%s)", pc_errname(errno));
	v = -77;
	pc_check(sem_getvalue(b, &v) == 0, "sem_getvalue() failed");
	pc_check(v == 2, "after sem_close() and a new sem_open(), the value is %d, expected 2", v);
	expect_count(b, 2, "after sem_close() and a new sem_open()");
	pc_check(sem_close(b) == 0, "second sem_close() failed");
	pc_pass();
}
