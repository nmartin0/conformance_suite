/*
 * A timed wait hit by a signal either fails with EINTR, leaving the
 * semaphore as it was, or goes on and times out; it never returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.008
 */
#include "semh.h"

struct targ {
	sem_t *s;
	const struct wf *w;
	struct timespec abs;
};

static int timed_job(void *p)
{
	struct targ *a = p;

	return do_timed(a->w, a->s, &a->abs);
}

int main(void)
{
	struct timespec after;
	struct targ a;
	struct job j;
	sem_t s;
	size_t i;

	pc_begin("XSH.sem_clockwait.008");
	guard(50);
	new_sem(&s, 0, 0);
	catch_usr1();
	pc_test();
	for (i = 0; i < 3; i++) {
		a.s = &s;
		a.w = &wfs[i];
		a.abs = deadline(&wfs[i], 1500);
		job_start(&j, timed_job, &a);
		sleep_ms(100);
		signal_job(&j);
		job_finish(&j, wfs[i].name);
		clk_now(wfs[i].clk, &after);
		pc_check(j.result == -1, "%s returned %d with no post made, expected -1", wfs[i].name, j.result);
		pc_check(j.err == EINTR || (j.err == ETIMEDOUT && ts_ge(&after, &a.abs)),
		    "%s: errno is %s, expected EINTR (or ETIMEDOUT once the time had passed)", wfs[i].name, rn(j.err));
		expect_count(&s, 0, "after a timed wait that was signalled");
	}
	pc_pass();
}
