/*
 * A post made while a timed wait is pending ends the wait with success.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_clockwait.005
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
	struct targ a;
	struct job j;
	sem_t s;
	size_t i;
	int r;

	pc_begin("XSH.sem_clockwait.005");
	guard(50);
	new_sem(&s, 0, 0);
	pc_test();
	for (i = 0; i < 3; i++) {
		a.s = &s;
		a.w = &wfs[i];
		a.abs = deadline(&wfs[i], 30000);
		job_start(&j, timed_job, &a);
		sleep_ms(100);
		pc_check(!atomic_load(&j.done), "%s returned (%d, %s) with no post made", wfs[i].name, j.result,
		    rn(j.err));
		r = sem_post(&s);
		pc_check(r == 0, "sem_post() returned %d", r);
		job_finish(&j, wfs[i].name);
		pc_check(j.result == 0, "%s returned %d (%s) after a post, expected 0", wfs[i].name, j.result, rn(j.err));
		expect_count(&s, 0, "after the timed wait took the post");
	}
	pc_pass();
}
