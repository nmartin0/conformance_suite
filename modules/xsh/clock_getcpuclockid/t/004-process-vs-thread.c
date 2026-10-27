/*
 * The process CPU-time clock is not below the clock of its only thread read
 * just before.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getcpuclockid.004
 */
#include "cpuh.h"
#include <pthread.h>

int main(void)
{
	clockid_t tid, pid;
	long long t, p;
	int e;

	pc_begin("XSH.clock_getcpuclockid.004");
	e = pthread_getcpuclockid(pthread_self(), &tid);
	if (e != 0)
		pc_unresolved("pthread_getcpuclockid failed: %s", pc_errname(e));
	if (clock_getcpuclockid(0, &pid) != 0)
		pc_unresolved("clock_getcpuclockid(0) failed: %s", pc_errname(errno));
	if (cpu_burn(tid) == 0)
		pc_unresolved("the thread clock did not advance");
	pc_test();
	t = cpu_ns(tid);
	p = cpu_ns(pid);
	pc_check(t >= 0 && p >= 0, "a CPU-time clock could not be read");
	pc_check(p >= t, "process clock %lld ns is below the thread clock %lld ns read before it", p, t);
	pc_pass();
}
