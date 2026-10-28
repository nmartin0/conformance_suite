/*
 * A shared memory segment's mode is applied by the class (user, group, other) that matches the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.7-xsi-ipc.004
 */
#include "pcipc.h"
#include <sys/shm.h>

/* Child: attach for reading, then for reading and writing; both must give the same verdict. */
static int try_ops(void *p)
{
	int id = *(int *)p, v1, v2;
	void *a;

	a = shmat(id, NULL, SHM_RDONLY);
	v1 = verdict(a != (void *)-1, errno);
	if (a != (void *)-1)
		(void)shmdt(a);
	a = shmat(id, NULL, 0);
	v2 = verdict(a != (void *)-1, errno);
	if (a != (void *)-1)
		(void)shmdt(a);
	return v1 == v2 ? v1 : R_OTHER;
}

static int hid = -1;

static void cleanup(void)
{
	if (hid >= 0)
		(void)shmctl(hid, IPC_RMID, NULL);
}

static void set_owner_mode(unsigned mode)
{
	struct shmid_ds ds;

	NEED(shmctl(hid, IPC_STAT, &ds) == 0, "IPC_STAT failed: %s", pc_errname(errno));
	ds.shm_perm.uid = ID_A;
	ds.shm_perm.gid = ID_B;
	ds.shm_perm.mode = mode;
	NEED(shmctl(hid, IPC_SET, &ds) == 0, "IPC_SET failed: %s", pc_errname(errno));
}

int main(void)
{
	int sel, kind, v;

	pc_begin("XSH.2.7-xsi-ipc.004");
	NEED(geteuid() == 0, "the test needs root to change user and group IDs");
	(void)signal(SIGALRM, ipc_on_alarm);
	hid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | IPC_EXCL | 0600);
	NEED(hid >= 0, "shmget failed: %s", pc_errname(errno));
	(void)atexit(cleanup);
	pc_test();
	(void)alarm(55);
	for (sel = 0; sel < 8; sel++) {
		for (kind = 0; kind < NKINDS; kind++) {
			int want = class_set(kind, sel) ? R_ALLOWED : R_DENIED;

			set_owner_mode(make_mode(sel & 4, sel & 2, sel & 1));
			v = run_as(kind_uid(kind), kind_gid(kind), try_ops, &hid);
			NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
			pc_check(v == want, "mode %03o, process %s: attaching %s, expected %s",
			    make_mode(sel & 4, sel & 2, sel & 1), kind_name(kind), verdict_name(v), verdict_name(want));
		}
	}
	pc_pass();
}
