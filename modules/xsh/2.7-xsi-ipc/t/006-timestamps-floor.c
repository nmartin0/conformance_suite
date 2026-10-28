/*
 * IPC timestamps are the whole second of the clock, not rounded up.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.7-xsi-ipc.006
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

union semun { int val; struct semid_ds *buf; unsigned short *array; };

static int qid = -1, sid = -1, hid = -1;
static time_t t_before, t_after;

static void cleanup(void)
{
	if (qid >= 0)
		(void)msgctl(qid, IPC_RMID, NULL);
	if (sid >= 0)
		(void)semctl(sid, 0, IPC_RMID);
	if (hid >= 0)
		(void)shmctl(hid, IPC_RMID, NULL);
}

/* Sleep until the fraction of the second is late in it (0.80 to 0.95), then note the second. */
static void late_in_second(void)
{
	int i;
	struct timespec now, nap = { 0, 5000000 };

	for (i = 0; i < 400; i++) {
		NEED(clock_gettime(CLOCK_REALTIME, &now) == 0, "clock_gettime failed");
		if (now.tv_nsec >= 800000000L && now.tv_nsec <= 950000000L) {
			t_before = now.tv_sec;
			return;
		}
		(void)nanosleep(&nap, NULL);
	}
	pc_unresolved("the clock never showed the late part of a second");
}

static void note_after(void)
{
	struct timespec now;

	NEED(clock_gettime(CLOCK_REALTIME, &now) == 0, "clock_gettime failed");
	t_after = now.tv_sec;
}

static void check(const char *what, time_t got)
{
	pc_check(got >= t_before && got <= t_after,
	    "%s is %ld; the clock showed %ld before the call and %ld after it", what, (long)got,
	    (long)t_before, (long)t_after);
}

int main(void)
{
	struct msqid_ds md;
	struct semid_ds sd;
	struct shmid_ds hd;
	union semun u;
	struct { long mtype; char mtext[2]; } m;
	struct sembuf op;
	void *a;

	pc_begin("XSH.2.7-xsi-ipc.006");
	qid = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0600);
	sid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
	hid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | IPC_EXCL | 0600);
	(void)atexit(cleanup);
	NEED(qid >= 0 && sid >= 0 && hid >= 0, "creating the objects failed: %s", pc_errname(errno));
	pc_test();
	m.mtype = 1;
	m.mtext[0] = 'x';

	late_in_second();
	NEED(msgsnd(qid, &m, 1, IPC_NOWAIT) == 0, "msgsnd failed: %s", pc_errname(errno));
	note_after();
	NEED(msgctl(qid, IPC_STAT, &md) == 0, "IPC_STAT failed");
	check("msg_stime after msgsnd()", md.msg_stime);

	late_in_second();
	NEED(msgrcv(qid, &m, 1, 0, IPC_NOWAIT) == 1, "msgrcv failed: %s", pc_errname(errno));
	note_after();
	NEED(msgctl(qid, IPC_STAT, &md) == 0, "IPC_STAT failed");
	check("msg_rtime after msgrcv()", md.msg_rtime);

	late_in_second();
	NEED(msgctl(qid, IPC_SET, &md) == 0, "IPC_SET failed: %s", pc_errname(errno));
	note_after();
	NEED(msgctl(qid, IPC_STAT, &md) == 0, "IPC_STAT failed");
	check("msg_ctime after msgctl(IPC_SET)", md.msg_ctime);

	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	late_in_second();
	NEED(semop(sid, &op, 1) == 0, "semop failed: %s", pc_errname(errno));
	note_after();
	u.buf = &sd;
	NEED(semctl(sid, 0, IPC_STAT, u) == 0, "IPC_STAT failed");
	check("sem_otime after semop()", sd.sem_otime);

	late_in_second();
	a = shmat(hid, NULL, 0);
	NEED(a != (void *)-1, "shmat failed: %s", pc_errname(errno));
	note_after();
	NEED(shmctl(hid, IPC_STAT, &hd) == 0, "IPC_STAT failed");
	check("shm_atime after shmat()", hd.shm_atime);

	late_in_second();
	NEED(shmdt(a) == 0, "shmdt failed: %s", pc_errname(errno));
	note_after();
	NEED(shmctl(hid, IPC_STAT, &hd) == 0, "IPC_STAT failed");
	check("shm_dtime after shmdt()", hd.shm_dtime);
	pc_pass();
}
