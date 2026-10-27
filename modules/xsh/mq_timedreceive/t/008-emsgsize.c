/*
 * A buffer smaller than mq_msgsize fails with EMSGSIZE and removes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.008
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_timedreceive.008");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(q, "short", 5, 0) == 0, "mq_send failed");
	pc_test();
	to = abs_in(5000);
	errno = 0;
	pc_check(mq_timedreceive(q, buf, 63, NULL, &to) == -1, "mq_timedreceive with a 63-byte buffer succeeded");
	pc_check(errno == EMSGSIZE, "errno is %s, not EMSGSIZE", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message was removed despite the error");
	pc_check(mq_timedreceive(q, buf, 64, NULL, &to) == 5, "a 64-byte buffer did not receive the message");
	pc_pass();
}
