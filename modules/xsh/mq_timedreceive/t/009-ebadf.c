/*
 * mq_timedreceive() with an invalid descriptor, or one not open for reading, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.009
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	char buf[64];
	const char *n;
	mqd_t q, w;

	pc_begin("XSH.mq_timedreceive.009");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	w = mq_again(n, O_WRONLY);
	NEED(mq_send(q, "m", 1, 0) == 0, "mq_send failed");
	pc_test();
	to = abs_in(5000);
	errno = 0;
	pc_check(mq_timedreceive(BADQ, buf, sizeof buf, NULL, &to) == -1, "mq_timedreceive on an invalid descriptor succeeded");
	pc_check(errno == EBADF, "invalid descriptor: errno is %s, not EBADF", pc_errname(errno));
	errno = 0;
	pc_check(mq_timedreceive(w, buf, sizeof buf, NULL, &to) == -1, "mq_timedreceive on a write-only descriptor succeeded");
	pc_check(errno == EBADF, "write-only descriptor: errno is %s, not EBADF", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message was removed despite the error");
	pc_pass();
}
