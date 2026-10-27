/*
 * A full queue freed before abstime lets a waiting mq_timedsend() succeed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.006
 */
#include "mqh.h"

int main(void)
{
	struct timespec to, t0;
	char buf[8];
	const char *n;
	pid_t kid;
	mqd_t q;
	long ms;
	int r, e, st;

	pc_begin("XSH.mq_timedsend.006");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 2, 8);
	fill(q, 2);
	alarm_handler_on();
	kid = fork();
	NEED(kid != -1, "fork failed");
	if (kid == 0) {
		sleep_ms(300);
		_exit(mq_receive(q, buf, sizeof buf, NULL) == 1 ? 0 : 1);
	}
	pc_test();
	NEED(clock_gettime(CLOCK_REALTIME, &t0) == 0, "clock_gettime failed");
	to = abs_in(10000);
	alarm(15);
	r = mq_timedsend(q, "n", 1, 0, &to);
	e = errno;
	alarm(0);
	ms = since_ms(&t0);
	st = reap(kid);
	NEED(st == 0, "the child could not receive");
	pc_check(r == 0, "mq_timedsend returned %d (%s) instead of waiting for room", r, pc_errname(e));
	pc_check(ms >= 150, "mq_timedsend returned after %ld ms, before the child made room", ms);
	pc_check(curmsgs(q) == 2, "mq_curmsgs is %ld, not 2", curmsgs(q));
	pc_pass();
}
