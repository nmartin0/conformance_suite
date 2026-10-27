/*
 * mq_setattr() stores the previous attributes and the current status in omqstat.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_setattr.003
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr in, old;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_setattr.003");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	fill(q, 2);
	memset(&in, 0, sizeof in);
	in.mq_flags = O_NONBLOCK;
	memset(&old, 0x55, sizeof old);
	pc_test();
	pc_check(mq_setattr(q, &in, &old) == 0, "mq_setattr failed: %s", pc_errname(errno));
	pc_check((old.mq_flags & O_NONBLOCK) == 0, "the previous mq_flags has O_NONBLOCK");
	pc_check(old.mq_maxmsg == 4 && old.mq_msgsize == 64, "previous sizes are %ld/%ld",
	    (long)old.mq_maxmsg, (long)old.mq_msgsize);
	pc_check(old.mq_curmsgs == 2, "mq_curmsgs is %ld, not 2", (long)old.mq_curmsgs);
	in.mq_flags = 0;
	memset(&old, 0x55, sizeof old);
	pc_check(mq_setattr(q, &in, &old) == 0, "the second mq_setattr failed: %s", pc_errname(errno));
	pc_check(old.mq_flags & O_NONBLOCK, "the previous mq_flags lacks O_NONBLOCK set by the first call");
	pc_pass();
}
