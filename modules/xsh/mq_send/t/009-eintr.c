/*
 * A signal interrupting a blocked mq_send() makes it fail with EINTR, queuing nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.009
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	mqd_t q;
	int r, e;

	pc_begin("XSH.mq_send.009");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 2, 8);
	fill(q, 2);
	alarm_handler_on();
	pc_test();
	alarm(1);
	r = mq_send(q, "n", 1, 0);
	e = errno;
	alarm(0);
	pc_check(r == -1, "mq_send returned %d on a full queue", r);
	pc_check(e == EINTR, "errno is %s, not EINTR", pc_errname(e));
	pc_check(curmsgs(q) == 2, "mq_curmsgs is %ld, not 2", curmsgs(q));
	pc_pass();
}
