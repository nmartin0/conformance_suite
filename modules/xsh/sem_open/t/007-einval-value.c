/*
 * sem_open() with O_CREAT and an initial value above {SEM_VALUE_MAX}
 * fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.007
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a;
	long max;

	pc_begin("XSH.sem_open.007");
	guard(30);
	max = semmax();
	NEED(max > 0, "{SEM_VALUE_MAX} is not determinate");
	if ((unsigned long)max >= UINT_MAX)
		pc_notinuse("no unsigned value exceeds {SEM_VALUE_MAX}");
	n = sn_make("a");
	pc_test();
	errno = 0;
	a = sem_open(n, O_CREAT | O_EXCL, (mode_t)0600, (unsigned)max + 1u);
	if (a != SEM_FAILED)
		pc_check(0, "sem_open() with value {SEM_VALUE_MAX}+1 succeeded");
	pc_check(errno == EINVAL, "errno is %s, expected EINVAL", pc_errname(errno));
	pc_pass();
}
