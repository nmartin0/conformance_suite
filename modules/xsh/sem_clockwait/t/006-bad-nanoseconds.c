/*
 * A timed wait that would block fails with EINVAL when the nanoseconds
 * field of the absolute time is below zero or not below 1000 million.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.006
 */
#include "semh.h"

int main(void)
{
	static const long bad[] = { -1L, 1000000000L, 2000000000L };
	struct timespec abs;
	sem_t s;
	size_t i, k;
	int r;

	pc_begin("XSH.sem_clockwait.006");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	for (i = 0; i < 3; i++) {
		for (k = 0; k < 3; k++) {
			abs = deadline(&wfs[i], 3000);
			abs.tv_nsec = bad[k];
			errno = 0;
			r = do_timed(&wfs[i], &s, &abs);
			pc_check(r == -1, "%s with tv_nsec %ld returned %d, expected -1", wfs[i].name, bad[k], r);
			pc_check(errno == EINVAL, "%s with tv_nsec %ld: errno is %s, expected EINVAL", wfs[i].name,
			    bad[k], pc_errname(errno));
		}
	}
	pc_pass();
}
