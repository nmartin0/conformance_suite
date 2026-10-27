/*
 * A queue opened O_RDWR both sends and receives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.012
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_open.012");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	pc_test();
	pc_check(mq_send(q, "rw", 2, 0) == 0, "mq_send failed: %s", pc_errname(errno));
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 2 && memcmp(buf, "rw", 2) == 0, "mq_receive failed");
	pc_pass();
}
