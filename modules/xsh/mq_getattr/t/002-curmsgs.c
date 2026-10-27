/*
 * mq_curmsgs follows the messages sent and received.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_getattr.002
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q, w;

	pc_begin("XSH.mq_getattr.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	w = mq_again(n, O_WRONLY);
	pc_test();
	pc_check(curmsgs(q) == 0, "not 0 at the start");
	fill(q, 3);
	pc_check(curmsgs(q) == 3, "mq_curmsgs is %ld after 3 sends", curmsgs(q));
	pc_check(curmsgs(w) == 3, "mq_curmsgs through a write-only descriptor is %ld, not 3", curmsgs(w));
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 1, "mq_receive failed");
	pc_check(curmsgs(q) == 2, "mq_curmsgs is %ld after a receive", curmsgs(q));
	pc_pass();
}
