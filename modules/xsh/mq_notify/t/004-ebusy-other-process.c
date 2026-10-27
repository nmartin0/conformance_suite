/*
 * If another process is registered, mq_notify() fails with EBUSY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.004
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;
	sigset_t set;
	const char *n;
	pid_t kid;
	mqd_t q;
	int st;

	pc_begin("XSH.mq_notify.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	rt_block(&set);
	ev = ev_signal(1);
	NEED(mq_notify(q, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	kid = fork();
	NEED(kid != -1, "fork failed");
	if (kid == 0) {
		mqd_t c = mq_open(n, O_RDWR);

		if (c == BADQ)
			_exit(3);
		errno = 0;
		if (mq_notify(c, &ev) == 0)
			_exit(1);
		_exit(errno == EBUSY ? 0 : 2);
	}
	pc_test();
	st = reap(kid);
	pc_check(st != 3, "the child could not open the queue");
	pc_check(st != 1, "a second process registered while the first was registered");
	pc_check(st == 0, "the second process's mq_notify failed with an error other than EBUSY");
	pc_pass();
}
