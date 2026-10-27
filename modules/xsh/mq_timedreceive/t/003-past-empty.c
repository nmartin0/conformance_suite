/*
 * On an empty queue an abstime already passed gives ETIMEDOUT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.003
 */
#include "mqh.h"

int main(void)
{
	struct timespec past;
	char buf[64];
	const char *n;
	mqd_t q;
	ssize_t r;
	int e;

	pc_begin("XSH.mq_timedreceive.003");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	alarm_handler_on();
	pc_test();
	past = abs_in(-2000);
	alarm(10);
	r = mq_timedreceive(q, buf, sizeof buf, NULL, &past);
	e = errno;
	alarm(0);
	pc_check(r == -1, "mq_timedreceive on an empty queue returned %ld", (long)r);
	pc_check(e == ETIMEDOUT, "errno is %s, not ETIMEDOUT", pc_errname(e));
	pc_pass();
}
