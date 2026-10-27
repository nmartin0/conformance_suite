/*
 * A message of mq_msgsize bytes is accepted; a longer one fails with EMSGSIZE and is not queued.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.004
 */
#include "mqh.h"

int main(void)
{
	char big[65], buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_send.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	memset(big, 'z', sizeof big);
	pc_test();
	pc_check(mq_send(q, big, 64, 0) == 0, "mq_send of 64 bytes failed: %s", pc_errname(errno));
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 64, "the message of 64 bytes did not come back whole");
	errno = 0;
	pc_check(mq_send(q, big, 65, 0) == -1, "mq_send of 65 bytes succeeded");
	pc_check(errno == EMSGSIZE, "errno is %s, not EMSGSIZE", pc_errname(errno));
	pc_check(curmsgs(q) == 0, "a message was queued despite the error");
	pc_pass();
}
