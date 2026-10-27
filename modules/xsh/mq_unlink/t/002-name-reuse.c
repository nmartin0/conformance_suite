/*
 * After mq_unlink() the name can create a new, empty queue; the old one lives on for its descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_unlink.002
 */
#include "mqh.h"

static int unlink_open(const char *n)
{
	int r;

	alarm_handler_on();
	alarm(3);
	r = mq_unlink(n);
	if (r == -1 && errno == EINTR)
		pc_unresolved("mq_unlink waited for the last close of the queue");
	alarm(0);
	return r;
}

int main(void)
{
	struct mq_attr at;
	char buf[64];
	const char *n;
	mqd_t old, fresh;

	pc_begin("XSH.mq_unlink.002");
	n = mq_name("a");
	old = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(old, "old", 3, 0) == 0, "mq_send failed");
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 2;
	at.mq_msgsize = 8;
	pc_test();
	pc_check(unlink_open(n) == 0, "mq_unlink failed: %s", pc_errname(errno));
	fresh = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
	pc_check(fresh != BADQ, "creating the name again failed: %s", pc_errname(errno));
	pc_check(curmsgs(fresh) == 0, "the new queue is not empty");
	pc_check(curmsgs(old) == 1, "the old queue lost its message");
	pc_check(mq_receive(old, buf, sizeof buf, NULL) == 3 && memcmp(buf, "old", 3) == 0,
	    "the old descriptor no longer reaches the old message");
	pc_pass();
}
