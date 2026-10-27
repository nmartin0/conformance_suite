/*
 * mq_setattr() accepts a NULL omqstat.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_setattr.004
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr in, now;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_setattr.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	memset(&in, 0, sizeof in);
	in.mq_flags = O_NONBLOCK;
	pc_test();
	pc_check(mq_setattr(q, &in, NULL) == 0, "mq_setattr with omqstat NULL failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0 && (now.mq_flags & O_NONBLOCK), "O_NONBLOCK was not set");
	pc_pass();
}
