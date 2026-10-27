/*
 * On a full queue an abstime already passed gives ETIMEDOUT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.003
 */
#include "mqh.h"

int main(void)
{
	struct timespec past;
	const char *n;
	mqd_t q;
	int r, e;

	pc_begin("XSH.mq_timedsend.003");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 2, 8);
	fill(q, 2);
	alarm_handler_on();
	pc_test();
	past = abs_in(-2000);
	alarm(10);
	r = mq_timedsend(q, "n", 1, 0, &past);
	e = errno;
	alarm(0);
	pc_check(r == -1, "mq_timedsend on a full queue returned %d", r);
	pc_check(e == ETIMEDOUT, "errno is %s, not ETIMEDOUT", pc_errname(e));
	pc_check(curmsgs(q) == 2, "mq_curmsgs is %ld, not 2", curmsgs(q));
	pc_pass();
}
