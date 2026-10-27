/*
 * sem_init() accepts the value {SEM_VALUE_MAX} itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_init.006
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	long max;
	int r, v = -1;

	pc_begin("XSH.sem_init.006");
	guard(30);
	max = semmax();
	NEED(max > 0, "{SEM_VALUE_MAX} is not determinate");
	NEED(max <= INT_MAX, "{SEM_VALUE_MAX} does not fit the int of sem_getvalue()");
	pc_test();
	r = sem_init(&s, 0, (unsigned)max);
	pc_check(r == 0, "sem_init(value {SEM_VALUE_MAX}) returned %d (%s), expected 0", r,
	    r == -1 ? pc_errname(errno) : "-");
	r = sem_getvalue(&s, &v);
	pc_check(r == 0, "sem_getvalue returned %d", r);
	pc_check(v == (int)max, "the value is %d, expected {SEM_VALUE_MAX} = %ld", v, max);
	pc_pass();
}
