/*
 * The value sem_getvalue() reports follows posts and waits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_getvalue.003
 */
#include "semh.h"

static int get(sem_t *s)
{
	int v = -77, r = sem_getvalue(s, &v);

	pc_check(r == 0, "sem_getvalue() returned %d (%s)", r, pc_errname(errno));
	return v;
}

int main(void)
{
	sem_t s;
	int v, i;

	pc_begin("XSH.sem_getvalue.003");
	guard(30);
	new_sem(&s, 0, 5);
	pc_test();
	v = get(&s);
	pc_check(v == 5, "initial value %d, expected 5", v);
	NEED(sem_wait(&s) == 0, "sem_wait failed");
	v = get(&s);
	pc_check(v == 4, "after a wait, value %d, expected 4", v);
	NEED(sem_post(&s) == 0, "sem_post failed");
	v = get(&s);
	pc_check(v == 5, "after a post, value %d, expected 5", v);
	for (i = 0; i < 5; i++)
		NEED(sem_trywait(&s) == 0, "sem_trywait failed");
	v = get(&s);
	pc_check(v == 0, "after five trywaits, value %d, expected 0", v);
	NEED(sem_post(&s) == 0, "sem_post failed");
	v = get(&s);
	pc_check(v == 1, "after a post of a locked semaphore, value %d, expected 1", v);
	pc_pass();
}
