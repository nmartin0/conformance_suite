/*
 * sem_post() on a semaphore at {SEM_VALUE_MAX} fails with EOVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_post.004
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	long max;
	int r;

	pc_begin("XSH.sem_post.004");
	guard(30);
	max = semmax();
	NEED(max > 0, "{SEM_VALUE_MAX} is not determinate");
	NEED(max <= UINT_MAX, "{SEM_VALUE_MAX} does not fit an unsigned value");
	new_sem(&s, 0, (unsigned)max);
	pc_test();
	r = sem_post(&s);
	pc_check(r == -1, "sem_post() at {SEM_VALUE_MAX} returned %d, expected -1", r);
	pc_check(errno == EOVERFLOW, "errno is %s, expected EOVERFLOW", pc_errname(errno));
	pc_pass();
}
