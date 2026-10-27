/*
 * clock_getcpuclockid(getpid()) gives a readable clock that does not go
 * backwards.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getcpuclockid.002
 */
#include "cpuh.h"
#include <unistd.h>

int main(void)
{
	clockid_t id;
	long long prev, now;
	volatile unsigned long x = 0;
	unsigned long k;
	int e, i;

	pc_begin("XSH.clock_getcpuclockid.002");
	pc_test();
	id = (clockid_t)-12345;
	e = clock_getcpuclockid(getpid(), &id);
	pc_check(e == 0, "clock_getcpuclockid(getpid()) failed: %s", pc_errname(e));
	prev = cpu_ns(id);
	pc_check(prev >= 0, "clock_gettime on the returned clock failed or gave an invalid time");
	for (i = 0; i < 50; i++) {
		for (k = 0; k < 100000; k++)
			x += k;
		now = cpu_ns(id);
		pc_check(now >= 0, "read %d of the clock failed", i);
		pc_check(now >= prev, "read %d (%lld ns) is below the previous one (%lld ns)", i, now, prev);
		prev = now;
	}
	pc_pass();
}
