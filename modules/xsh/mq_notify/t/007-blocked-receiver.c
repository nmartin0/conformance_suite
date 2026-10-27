/*
 * A thread blocked in mq_receive() gets the arriving message; no notification is sent and the registration stays.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.007
 */
#include "mqh.h"
#include <stdatomic.h>

static mqd_t qr;
static atomic_int done, got, started;

static void *receiver(void *arg)
{
	char buf[64];

	(void)arg;
	atomic_store(&started, 1);
	atomic_store(&got, (int)mq_receive(qr, buf, sizeof buf, NULL));
	atomic_store(&done, 1);
	return NULL;
}

int main(void)
{
	struct sigevent ev;
	siginfo_t info;
	sigset_t set;
	pthread_t th;
	const char *n;
	mqd_t q;
	int i;

	pc_begin("XSH.mq_notify.007");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	qr = mq_again(n, O_RDWR);
	rt_block(&set);
	ev = ev_signal(3);
	NEED(pthread_create(&th, NULL, receiver, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 500 && !atomic_load(&started); i++)
		sleep_ms(10);
	sleep_ms(300);	/* time for the thread to block in mq_receive() */
	NEED(mq_notify(q, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	pc_test();
	pc_check(mq_send(q, "m", 1, 0) == 0, "mq_send failed");
	for (i = 0; i < 500 && !atomic_load(&done); i++)
		sleep_ms(10);
	if (!atomic_load(&done)) {
		pc_fail("the blocked mq_receive did not get the arriving message");
	}
	(void)pthread_join(th, NULL);
	pc_check(atomic_load(&got) == 1, "the blocked mq_receive returned %d", atomic_load(&got));
	pc_check(!rt_wait(&set, &info, 300), "a notification came although a receiver was blocked");
	errno = 0;
	pc_check(mq_notify(q, &ev) == -1 && errno == EBUSY, "the registration did not stay: errno %s", pc_errname(errno));
	pc_pass();
}
