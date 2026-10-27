/*
 * A message sent before abstime ends a waiting mq_timedreceive() with success.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.006
 */
#include "mqh.h"

int main(void)
{
	struct timespec to, t0;
	char buf[64];
	const char *n;
	pid_t kid;
	mqd_t q;
	long ms;
	ssize_t r;
	int e, st;

	pc_begin("XSH.mq_timedreceive.006");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	alarm_handler_on();
	kid = fork();
	NEED(kid != -1, "fork failed");
	if (kid == 0) {
		sleep_ms(300);
		_exit(mq_send(q, "late", 4, 0) == 0 ? 0 : 1);
	}
	pc_test();
	NEED(clock_gettime(CLOCK_REALTIME, &t0) == 0, "clock_gettime failed");
	to = abs_in(10000);
	alarm(15);
	r = mq_timedreceive(q, buf, sizeof buf, NULL, &to);
	e = errno;
	alarm(0);
	ms = since_ms(&t0);
	st = reap(kid);
	NEED(st == 0, "the child could not send");
	pc_check(r == 4, "mq_timedreceive returned %ld (%s) instead of waiting for a message", (long)r, pc_errname(e));
	pc_check(memcmp(buf, "late", 4) == 0, "the content differs");
	pc_check(ms >= 150, "mq_timedreceive returned after %ld ms, before a message was sent", ms);
	pc_pass();
}
