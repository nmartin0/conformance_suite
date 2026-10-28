/*
 * A semaphore set's mode is applied by the class (user, group, other) that matches the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.7-xsi-ipc.003
 */
#include "pcipc.h"
#include <sys/sem.h>

union semun { int val; struct semid_ds *buf; unsigned short *array; };

/* Child: read (GETVAL) and alter (+1) must give the same verdict. */
static int try_ops(void *p)
{
	int id = *(int *)p, v1, v2;
	struct sembuf op;
	int ok;

	ok = (semctl(id, 0, GETVAL) >= 0);
	v1 = verdict(ok, errno);
	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = IPC_NOWAIT;
	ok = (semop(id, &op, 1) == 0);
	v2 = verdict(ok, errno);
	return v1 == v2 ? v1 : R_OTHER;
}

static int sid = -1;

static void cleanup(void)
{
	if (sid >= 0)
		(void)semctl(sid, 0, IPC_RMID);
}

static void set_owner_mode(unsigned mode)
{
	struct semid_ds ds;
	union semun u;

	u.buf = &ds;
	NEED(semctl(sid, 0, IPC_STAT, u) == 0, "IPC_STAT failed: %s", pc_errname(errno));
	ds.sem_perm.uid = ID_A;
	ds.sem_perm.gid = ID_B;
	ds.sem_perm.mode = mode;
	NEED(semctl(sid, 0, IPC_SET, u) == 0, "IPC_SET failed: %s", pc_errname(errno));
}

int main(void)
{
	int sel, kind, v;

	pc_begin("XSH.2.7-xsi-ipc.003");
	NEED(geteuid() == 0, "the test needs root to change user and group IDs");
	(void)signal(SIGALRM, ipc_on_alarm);
	sid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
	NEED(sid >= 0, "semget failed: %s", pc_errname(errno));
	(void)atexit(cleanup);
	pc_test();
	(void)alarm(55);
	for (sel = 0; sel < 8; sel++) {
		for (kind = 0; kind < NKINDS; kind++) {
			int want = class_set(kind, sel) ? R_ALLOWED : R_DENIED;

			set_owner_mode(make_mode(sel & 4, sel & 2, sel & 1));
			v = run_as(kind_uid(kind), kind_gid(kind), try_ops, &sid);
			NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
			pc_check(v == want, "mode %03o, process %s: operations %s, expected %s",
			    make_mode(sel & 4, sel & 2, sel & 1), kind_name(kind), verdict_name(v), verdict_name(want));
		}
	}
	pc_pass();
}
