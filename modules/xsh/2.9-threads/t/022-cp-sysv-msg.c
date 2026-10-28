/*
 * msgsnd() and msgrcv(), cancellation points, act on a pending
 * cancellation request before they return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.022
 *
 * See XSH.2.9-threads.012. Both calls use IPC_NOWAIT; the queue is
 * removed when the test ends.
 */
#include "pccp.h"
#include <sys/ipc.h>
#include <sys/msg.h>

static volatile long sink;
static int qid = -1;

struct mbuf {
	long mtype;
	char mtext[8];
};

static void rm_queue(void)
{
	if (qid != -1)
		(void)msgctl(qid, IPC_RMID, NULL);
}

static void drain(void)
{
	struct mbuf m;

	while (msgrcv(qid, &m, sizeof m.mtext, 0, IPC_NOWAIT) != -1)
		;
}

static void s_empty(void) { drain(); }
static void s_one(void)
{
	struct mbuf m;

	drain();
	m.mtype = 1;
	memcpy(m.mtext, "abc", 4);
	sink = msgsnd(qid, &m, 4, IPC_NOWAIT);
}

static void c_msgsnd(void)
{
	struct mbuf m;

	m.mtype = 1;
	memcpy(m.mtext, "abc", 4);
	sink = msgsnd(qid, &m, 4, IPC_NOWAIT);
}
static void c_msgrcv(void) { struct mbuf m; sink = msgrcv(qid, &m, sizeof m.mtext, 0, IPC_NOWAIT); }

static const struct cpcase cases[] = {
	{ "msgsnd", s_empty, c_msgsnd, NULL },
	{ "msgrcv", s_one, c_msgrcv, NULL },
};

int main(void)
{
	char bad[256];
	int n;

	pc_begin("XSH.2.9-threads.022");
	guard(120);
	qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
	NEED(qid != -1, "msgget failed: %s", pc_errname(errno));
	atexit(rm_queue);
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
