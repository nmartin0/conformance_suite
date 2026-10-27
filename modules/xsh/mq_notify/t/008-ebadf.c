/*
 * mq_notify() with an invalid descriptor fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.008
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;

	pc_begin("XSH.mq_notify.008");
	ev = ev_signal(1);
	pc_test();
	errno = 0;
	pc_check(mq_notify(BADQ, &ev) == -1, "mq_notify on an invalid descriptor succeeded");
	pc_check(errno == EBADF, "errno is %s, not EBADF", pc_errname(errno));
	pc_pass();
}
