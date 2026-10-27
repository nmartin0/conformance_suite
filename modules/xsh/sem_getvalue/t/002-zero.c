/*
 * sem_getvalue() of a locked semaphore with no waiters gives zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_getvalue.002
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	int r, v;

	pc_begin("XSH.sem_getvalue.002");
	guard(30);
	new_sem(&s, 0, 0);
	pc_test();
	v = -77;
	r = sem_getvalue(&s, &v);
	pc_check(r == 0, "sem_getvalue() returned %d (%s)", r, pc_errname(errno));
	pc_check(v == 0, "sem_getvalue() of a value 0 semaphore with no waiter gave %d, expected 0", v);
	NEED(sem_post(&s) == 0, "sem_post failed");
	NEED(sem_wait(&s) == 0, "sem_wait failed");
	v = -77;
	r = sem_getvalue(&s, &v);
	pc_check(r == 0 && v == 0, "after a post and a wait, sem_getvalue() gave %d (returned %d), expected 0", v, r);
	pc_pass();
}
