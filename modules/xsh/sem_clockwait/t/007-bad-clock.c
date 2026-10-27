/*
 * sem_clockwait() that would block fails with EINVAL for a clock_id that
 * names no clock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.007
 */
#include "semh.h"

/* Candidates, none defined by any system we know of. */
static const long candidates[] = { 0x7ffffff0L, 0x3ffffff7L, 0x12345679L, 0x5aa5a55aL };

int main(void)
{
	struct timespec abs, ts;
	sem_t s;
	size_t i;
	int r, e;

	pc_begin("XSH.sem_clockwait.007");
	guard(40);
	new_sem(&s, 0, 0);
	clk_now(CLOCK_REALTIME, &abs);
	ts_add_ms(&abs, 3000);
	pc_test();
	for (i = 0; i < sizeof candidates / sizeof candidates[0]; i++) {
		clockid_t id = (clockid_t)candidates[i];

		errno = 0;
		if (clock_gettime(id, &ts) == 0 || errno != EINVAL)
			continue;
		errno = 0;
		r = sem_clockwait(&s, id, &abs);
		e = errno;
		pc_check(r == -1, "sem_clockwait() with the unknown clock %#lx returned %d, expected -1", candidates[i], r);
		pc_check(e == EINVAL, "sem_clockwait() with the unknown clock %#lx: errno is %s, expected EINVAL",
		    candidates[i], pc_errname(e));
		pc_pass();
	}
	pc_unresolved("every candidate clock ID names a known clock here");
}
