/*
 * A buffer smaller than mq_msgsize fails with EMSGSIZE and removes nothing; an exact-size buffer works.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.003
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_receive.003");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(q, "short", 5, 0) == 0, "mq_send failed");
	pc_test();
	errno = 0;
	pc_check(mq_receive(q, buf, 63, NULL) == -1, "mq_receive with a 63-byte buffer succeeded");
	pc_check(errno == EMSGSIZE, "errno is %s, not EMSGSIZE", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message was removed despite the error");
	pc_check(mq_receive(q, buf, 64, NULL) == 5 && memcmp(buf, "short", 5) == 0,
	    "a 64-byte buffer did not receive the 5-byte message");
	pc_pass();
}
