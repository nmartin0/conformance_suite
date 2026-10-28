/*
 * Message queue functions listed as cancellation points act on a pending
 * cancellation request before they return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.020
 *
 * For each of mq_send(), mq_receive(), mq_timedsend() and
 * mq_timedreceive(): see XSH.2.9-threads.012. The queue is opened
 * non-blocking and its name removed at once.
 */
#include "pccp.h"
#include <fcntl.h>
#include <mqueue.h>

static volatile long sink;
static mqd_t q;

static void empty_queue(void)
{
	char buf[64];

	while (mq_receive(q, buf, sizeof buf, NULL) != -1)
		;
}

static void s_empty(void) { empty_queue(); }
static void s_one(void) { empty_queue(); sink = mq_send(q, "m", 1, 0); }

static void c_send(void) { sink = mq_send(q, "m", 1, 0); }
static void c_receive(void) { char buf[64]; sink = mq_receive(q, buf, sizeof buf, NULL); }
static void c_timedsend(void)
{
	struct timespec ts = ts_after(CLOCK_REALTIME, 30000);

	sink = mq_timedsend(q, "m", 1, 0, &ts);
}
static void c_timedreceive(void)
{
	char buf[64];
	struct timespec ts = ts_after(CLOCK_REALTIME, 30000);

	sink = mq_timedreceive(q, buf, sizeof buf, NULL, &ts);
}

static const struct cpcase cases[] = {
	{ "mq_send", s_empty, c_send, NULL },
	{ "mq_receive", s_one, c_receive, NULL },
	{ "mq_timedsend", s_empty, c_timedsend, NULL },
	{ "mq_timedreceive", s_one, c_timedreceive, NULL },
};

int main(void)
{
	char bad[512], name[64];
	struct mq_attr at;
	int n;

	pc_begin("XSH.2.9-threads.020");
	guard(300);
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 8;
	at.mq_msgsize = 32;
	(void)snprintf(name, sizeof name, "/pc-g2-cp-%ld", (long)getpid());
	q = mq_open(name, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK, 0600, &at);
	NEED(q != (mqd_t)-1, "mq_open failed: %s", pc_errname(errno));
	(void)mq_unlink(name);
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
