/*
 * mq_receive() with an invalid descriptor, or one not open for reading, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.005
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q, w;

	pc_begin("XSH.mq_receive.005");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	w = mq_again(n, O_WRONLY);
	NEED(mq_send(q, "m", 1, 0) == 0, "mq_send failed");
	pc_test();
	errno = 0;
	pc_check(mq_receive(BADQ, buf, sizeof buf, NULL) == -1, "mq_receive on an invalid descriptor succeeded");
	pc_check(errno == EBADF, "invalid descriptor: errno is %s, not EBADF", pc_errname(errno));
	errno = 0;
	pc_check(mq_receive(w, buf, sizeof buf, NULL) == -1, "mq_receive on a write-only descriptor succeeded");
	pc_check(errno == EBADF, "write-only descriptor: errno is %s, not EBADF", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message was removed despite the error");
	pc_pass();
}
