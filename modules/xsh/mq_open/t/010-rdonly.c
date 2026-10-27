/*
 * A queue opened O_RDONLY receives but does not send.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.010
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t w, r;

	pc_begin("XSH.mq_open.010");
	n = mq_name("a");
	w = mq_new(n, O_RDWR, 4, 64);
	r = mq_again(n, O_RDONLY);
	NEED(mq_send(w, "m", 1, 0) == 0, "mq_send failed");
	pc_test();
	errno = 0;
	pc_check(mq_send(r, "n", 1, 0) == -1, "mq_send on a read-only descriptor succeeded");
	pc_check(errno == EBADF, "errno is %s, not EBADF", pc_errname(errno));
	pc_check(mq_receive(r, buf, sizeof buf, NULL) == 1 && buf[0] == 'm', "mq_receive on a read-only descriptor failed");
	pc_pass();
}
