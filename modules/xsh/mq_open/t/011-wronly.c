/*
 * A queue opened O_WRONLY sends but does not receive.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.011
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q, w;

	pc_begin("XSH.mq_open.011");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	w = mq_again(n, O_WRONLY | O_NONBLOCK);
	pc_test();
	pc_check(mq_send(w, "m", 1, 0) == 0, "mq_send on a write-only descriptor failed: %s", pc_errname(errno));
	errno = 0;
	pc_check(mq_receive(w, buf, sizeof buf, NULL) == -1, "mq_receive on a write-only descriptor succeeded");
	pc_check(errno == EBADF, "errno is %s, not EBADF", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message is gone");
	pc_pass();
}
