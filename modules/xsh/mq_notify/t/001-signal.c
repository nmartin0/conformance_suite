/*
 * A SIGEV_SIGNAL registration delivers its signal, with the application value, when a message arrives at the empty queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.001
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;
	siginfo_t info;
	sigset_t set;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_notify.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	rt_block(&set);
	ev = ev_signal(42);
	pc_test();
	pc_check(mq_notify(q, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	pc_check(!rt_wait(&set, &info, 200), "the signal came before any message arrived");
	pc_check(mq_send(q, "m", 1, 0) == 0, "mq_send failed");
	pc_check(rt_wait(&set, &info, 5000), "no signal after a message arrived at the empty queue");
	pc_check(info.si_signo == SIGRTMIN, "the signal is %d", info.si_signo);
	pc_check(info.si_code == SI_MESGQ, "si_code is %d, not SI_MESGQ", info.si_code);
	pc_check(info.si_value.sival_int == 42, "si_value is %d, not 42", info.si_value.sival_int);
	pc_pass();
}
