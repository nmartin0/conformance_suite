/*
 * A timed wait on a semaphore of value 0, with an absolute time already
 * passed, fails with ETIMEDOUT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.003
 */
#include "semh.h"

int main(void)
{
	struct timespec abs;
	sem_t s;
	size_t i;
	int r, k;

	pc_begin("XSH.sem_clockwait.003");
	guard(40);
	new_sem(&s, 0, 0);
	pc_test();
	for (i = 0; i < 3; i++) {
		for (k = 0; k < 2; k++) {
			if (k == 0) {
				abs.tv_sec = 0;
				abs.tv_nsec = 0;
			} else {
				abs = deadline(&wfs[i], 0);
				if (abs.tv_sec < 2)
					continue;
				abs.tv_sec -= 1;
			}
			errno = 0;
			r = do_timed(&wfs[i], &s, &abs);
			pc_check(r == -1, "%s with a passed time (case %d) returned %d, expected -1", wfs[i].name, k, r);
			pc_check(errno == ETIMEDOUT, "%s (case %d): errno is %s, expected ETIMEDOUT", wfs[i].name, k,
			    pc_errname(errno));
		}
	}
	pc_pass();
}
