/*
 * Closing the descriptor that registered for notification frees the queue for another registration.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_close.003
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_close.003");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	q2 = mq_again(n, O_RDWR);
	memset(&ev, 0, sizeof ev);
	ev.sigev_notify = SIGEV_SIGNAL;
	ev.sigev_signo = SIGUSR1;
	{
		struct sigaction sa;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = nop_handler;
		sigemptyset(&sa.sa_mask);
		NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	}
	NEED(mq_notify(q1, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	errno = 0;
	NEED(mq_notify(q2, &ev) == -1 && errno == EBUSY, "a second registration was not refused with EBUSY");
	pc_test();
	pc_check(mq_close(q1) == 0, "mq_close failed");
	pc_check(mq_notify(q2, &ev) == 0, "registration after closing the registered descriptor failed: %s", pc_errname(errno));
	pc_pass();
}
