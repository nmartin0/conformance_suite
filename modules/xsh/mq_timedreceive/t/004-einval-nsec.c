/*
 * On an empty queue a nanoseconds field below 0 or 1000 million gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.004
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	char buf[64];
	const char *n;
	mqd_t q;
	long bad[2] = { -1, 1000000000L };
	ssize_t r;
	int i, e;

	pc_begin("XSH.mq_timedreceive.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	alarm_handler_on();
	pc_test();
	for (i = 0; i < 2; i++) {
		to = abs_in(2000);
		to.tv_nsec = bad[i];
		alarm(10);
		r = mq_timedreceive(q, buf, sizeof buf, NULL, &to);
		e = errno;
		alarm(0);
		pc_check(r == -1, "tv_nsec %ld: mq_timedreceive returned %ld", bad[i], (long)r);
		pc_check(e == EINVAL, "tv_nsec %ld: errno is %s, not EINVAL", bad[i], pc_errname(e));
	}
	pc_pass();
}
