/*
 * A message queue's mode is applied by the class (user, group, other) that matches the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.7-xsi-ipc.002
 */
#include "pcipc.h"
#include <sys/msg.h>

struct job { int id; int write_wanted, read_wanted; };
struct pmsg { long mtype; char mtext[4]; };

/* Child: try the operations asked for; they must all give the same verdict. */
static int try_ops(void *p)
{
	struct job *j = p;
	struct pmsg m, r;
	int v[2], n = 0, i;

	m.mtype = 1;
	m.mtext[0] = 'x';
	if (j->write_wanted) {
		int ok = (msgsnd(j->id, &m, 1, IPC_NOWAIT) == 0);

		v[n++] = verdict(ok, errno);
	}
	if (j->read_wanted) {
		int ok = (msgrcv(j->id, &r, 1, 0, IPC_NOWAIT | MSG_NOERROR) >= 0);

		/* an empty queue gives ENOMSG, which means the permission was there */
		v[n++] = (ok || errno == ENOMSG) ? R_ALLOWED : verdict(0, errno);
	}
	for (i = 1; i < n; i++)
		if (v[i] != v[0])
			return R_OTHER;
	return n > 0 ? v[0] : R_OTHER;
}

static void drain(int id)
{
	struct pmsg r;

	while (msgrcv(id, &r, 1, 0, IPC_NOWAIT | MSG_NOERROR) >= 0)
		;
}

static int qid = -1;

static void cleanup(void)
{
	if (qid >= 0)
		(void)msgctl(qid, IPC_RMID, NULL);
}

static void set_owner_mode(unsigned mode)
{
	struct msqid_ds ds;

	NEED(msgctl(qid, IPC_STAT, &ds) == 0, "IPC_STAT failed: %s", pc_errname(errno));
	ds.msg_perm.uid = ID_A;
	ds.msg_perm.gid = ID_B;
	ds.msg_perm.mode = mode;
	NEED(msgctl(qid, IPC_SET, &ds) == 0, "IPC_SET failed: %s", pc_errname(errno));
}

int main(void)
{
	int sel, kind, v;
	struct job j;

	pc_begin("XSH.2.7-xsi-ipc.002");
	NEED(geteuid() == 0, "the test needs root to change user and group IDs");
	(void)signal(SIGALRM, ipc_on_alarm);
	qid = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0600);
	NEED(qid >= 0, "msgget failed: %s", pc_errname(errno));
	(void)atexit(cleanup);
	pc_test();
	(void)alarm(55);
	j.id = qid;
	j.write_wanted = j.read_wanted = 1;
	for (sel = 0; sel < 8; sel++) {
		for (kind = 0; kind < NKINDS; kind++) {
			int want = class_set(kind, sel) ? R_ALLOWED : R_DENIED;

			set_owner_mode(make_mode(sel & 4, sel & 2, sel & 1));
			drain(qid);
			v = run_as(kind_uid(kind), kind_gid(kind), try_ops, &j);
			NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
			pc_check(v == want, "mode %03o, process %s: operations %s, expected %s",
			    make_mode(sel & 4, sel & 2, sel & 1), kind_name(kind), verdict_name(v), verdict_name(want));
		}
	}
	/* a single bit of the class: user write only, user read only */
	set_owner_mode(0200);
	drain(qid);
	j.write_wanted = 1;
	j.read_wanted = 0;
	v = run_as(ID_A, ID_Y, try_ops, &j);
	NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
	pc_check(v == R_ALLOWED, "mode 0200: msgsnd() by the owner was %s", verdict_name(v));
	drain(qid);
	j.write_wanted = 0;
	j.read_wanted = 1;
	v = run_as(ID_A, ID_Y, try_ops, &j);
	NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
	pc_check(v == R_DENIED, "mode 0200: msgrcv() by the owner was %s", verdict_name(v));
	set_owner_mode(0400);
	drain(qid);
	j.write_wanted = 1;
	j.read_wanted = 0;
	v = run_as(ID_A, ID_Y, try_ops, &j);
	NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
	pc_check(v == R_DENIED, "mode 0400: msgsnd() by the owner was %s", verdict_name(v));
	j.write_wanted = 0;
	j.read_wanted = 1;
	v = run_as(ID_A, ID_Y, try_ops, &j);
	NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
	pc_check(v == R_ALLOWED, "mode 0400: msgrcv() by the owner was %s", verdict_name(v));
	pc_pass();
}
