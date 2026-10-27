/*
 * A SIGEV_THREAD registration runs the function, with the application value, when a message arrives at the empty queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.010
 */
#include "mqh.h"
#include <stdatomic.h>

static atomic_int called, value;

static void fn(union sigval sv)
{
	atomic_store(&value, sv.sival_int);
	atomic_store(&called, 1);
}

int main(void)
{
	struct sigevent ev;
	char buf[64];
	const char *n;
	mqd_t q;
	int i;

	pc_begin("XSH.mq_notify.010");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	memset(&ev, 0, sizeof ev);
	ev.sigev_notify = SIGEV_THREAD;
	ev.sigev_notify_function = fn;
	ev.sigev_notify_attributes = NULL;
	ev.sigev_value.sival_int = 77;
	pc_test();
	pc_check(mq_notify(q, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	sleep_ms(200);
	pc_check(!atomic_load(&called), "the function ran before any message arrived");
	pc_check(mq_send(q, "m", 1, 0) == 0, "mq_send failed");
	for (i = 0; i < 500 && !atomic_load(&called); i++)
		sleep_ms(10);
	pc_check(atomic_load(&called), "the notification function did not run");
	pc_check(atomic_load(&value) == 77, "the function got %d, not 77", atomic_load(&value));
	(void)mq_receive(q, buf, sizeof buf, NULL);
	pc_pass();
}
