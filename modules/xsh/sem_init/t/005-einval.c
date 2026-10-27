/*
 * sem_init() with a value above {SEM_VALUE_MAX} fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_init.005
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	long max;
	int r;

	pc_begin("XSH.sem_init.005");
	guard(30);
	max = semmax();
	NEED(max > 0, "{SEM_VALUE_MAX} is not determinate");
	if ((unsigned long)max >= UINT_MAX)
		pc_notinuse("no unsigned value exceeds {SEM_VALUE_MAX}");
	pc_test();
	r = sem_init(&s, 0, (unsigned)max + 1u);
	pc_check(r == -1, "sem_init(value {SEM_VALUE_MAX}+1) returned %d, expected -1", r);
	pc_check(errno == EINVAL, "errno is %s, expected EINVAL", pc_errname(errno));
	r = sem_init(&s, 0, UINT_MAX);
	pc_check(r == -1, "sem_init(value UINT_MAX) returned %d, expected -1", r);
	pc_check(errno == EINVAL, "errno is %s, expected EINVAL", pc_errname(errno));
	pc_pass();
}
