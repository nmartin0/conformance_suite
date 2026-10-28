/*
 * IPC identifiers are unique positive integers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.7-xsi-ipc.001
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define N 10

static int mq[N], ss[N], hm[N];

static void cleanup(void)
{
	int i;

	for (i = 0; i < N; i++) {
		if (mq[i] >= 0)
			(void)msgctl(mq[i], IPC_RMID, NULL);
		if (ss[i] >= 0)
			(void)semctl(ss[i], 0, IPC_RMID);
		if (hm[i] >= 0)
			(void)shmctl(hm[i], IPC_RMID, NULL);
	}
}

int main(void)
{
	int i, j;

	pc_begin("XSH.2.7-xsi-ipc.001");
	for (i = 0; i < N; i++)
		mq[i] = ss[i] = hm[i] = -1;
	(void)atexit(cleanup);
	for (i = 0; i < N; i++) {
		mq[i] = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0600);
		NEED(mq[i] >= 0, "msgget failed: %s", pc_errname(errno));
		ss[i] = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
		NEED(ss[i] >= 0, "semget failed: %s", pc_errname(errno));
		hm[i] = shmget(IPC_PRIVATE, 4096, IPC_CREAT | IPC_EXCL | 0600);
		NEED(hm[i] >= 0, "shmget failed: %s", pc_errname(errno));
	}
	pc_test();
	for (i = 0; i < N; i++) {
		pc_check(mq[i] > 0, "message queue identifier %d is not positive", mq[i]);
		pc_check(ss[i] > 0, "semaphore set identifier %d is not positive", ss[i]);
		pc_check(hm[i] > 0, "shared memory identifier %d is not positive", hm[i]);
		for (j = i + 1; j < N; j++) {
			pc_check(mq[i] != mq[j], "two message queues have the identifier %d", mq[i]);
			pc_check(ss[i] != ss[j], "two semaphore sets have the identifier %d", ss[i]);
			pc_check(hm[i] != hm[j], "two shared memory segments have the identifier %d", hm[i]);
		}
	}
	pc_pass();
}
