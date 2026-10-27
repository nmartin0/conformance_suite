/*
 * On a full queue a nanoseconds field below 0 or 1000 million gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.004
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	const char *n;
	mqd_t q;
	long bad[2] = { -1, 1000000000L };
	int i, r, e;

	pc_begin("XSH.mq_timedsend.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 2, 8);
	fill(q, 2);
	alarm_handler_on();
	pc_test();
	for (i = 0; i < 2; i++) {
		to = abs_in(2000);
		to.tv_nsec = bad[i];
		alarm(10);
		r = mq_timedsend(q, "n", 1, 0, &to);
		e = errno;
		alarm(0);
		pc_check(r == -1, "tv_nsec %ld: mq_timedsend returned %d", bad[i], r);
		pc_check(e == EINVAL, "tv_nsec %ld: errno is %s, not EINVAL", bad[i], pc_errname(e));
	}
	pc_check(curmsgs(q) == 2, "mq_curmsgs is %ld, not 2", curmsgs(q));
	pc_pass();
}
