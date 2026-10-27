/*
 * sem_getvalue() reports the value of an unlocked semaphore and does not
 * change it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_getvalue.001
 */
#include "semh.h"

int main(void)
{
	static const unsigned vals[] = { 1, 5 };
	sem_t s;
	long max;
	size_t i;
	int r, v, n;

	pc_begin("XSH.sem_getvalue.001");
	guard(30);
	max = semmax();
	pc_test();
	n = max > 0 && max <= INT_MAX ? 3 : 2;
	for (i = 0; i < (size_t)n; i++) {
		unsigned want = i < 2 ? vals[i] : (unsigned)max;

		new_sem(&s, 0, want);
		v = -77;
		r = sem_getvalue(&s, &v);
		pc_check(r == 0, "sem_getvalue() returned %d (%s), expected 0", r, pc_errname(errno));
		pc_check(v == (int)want, "sem_getvalue() of a value %u semaphore gave %d", want, v);
		v = -77;
		r = sem_getvalue(&s, &v);
		pc_check(r == 0 && v == (int)want, "a second sem_getvalue() gave %d (returned %d), expected %u", v, r, want);
		if (want <= 5)
			expect_count(&s, (int)want, "after two sem_getvalue() calls");
		NEED(sem_destroy(&s) == 0, "sem_destroy failed");
	}
	pc_pass();
}
