/*
 * The clock ID clock_getcpuclockid() gives for the process works.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.009
 * MUTANT    clock_getres.cpuclockid_einval
 * MUTANT    clock_getres.cpuclockid_frozen
 */
#include "pc.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static long long ns_of(clockid_t id)
{
	struct timespec t;

	pc_check(clock_gettime(id, &t) == 0, "clock_gettime(clock_getcpuclockid's ID) failed: %s", pc_errname(errno));
	pc_check(t.tv_nsec >= 0 && t.tv_nsec < 1000000000L, "clock_gettime gave tv_nsec %ld", (long)t.tv_nsec);
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long wall(void)
{
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
		pc_unresolved("clock_gettime(CLOCK_MONOTONIC) failed");
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

int main(void)
{
	struct timespec res;
	clockid_t id;
	long long c0, c1, w0;
	volatile unsigned long spin = 0;
	int r;

	pc_begin("XSH.clock_getres.009");
	r = clock_getcpuclockid(getpid(), &id);
	NEED(r == 0, "clock_getcpuclockid(getpid()) returned %d (%s)", r, pc_errname(r));
	pc_test();
	{
		int r_, e_;

		errno = 0;
		r_ = clock_getres(id, &res);
		e_ = errno;
		pc_check(r_ == 0, "clock_getres(clock_getcpuclockid's ID) failed: %s", pc_errname(e_));
	}
	c0 = ns_of(id);
	w0 = wall();
	do {
		int k;
		for (k = 0; k < 100000; k++)
			spin++;
		c1 = ns_of(id);
	} while (c1 <= c0 && wall() - w0 < 5000000000LL);
	pc_check(c1 > c0, "the process's CPU-time clock did not advance while it computed for 5 s");
	pc_pass();
}
