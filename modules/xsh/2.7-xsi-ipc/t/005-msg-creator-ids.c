/*
 * The creator's user and group IDs, as well as the owner's, select the class of a message queue's mode.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.7-xsi-ipc.005
 */
#include "pcipc.h"
#include <sys/msg.h>

#define C_U 30021	/* creator's user */
#define C_G 30022	/* creator's group */
#define O_U 30023	/* new owner's user */
#define O_G 30024	/* new owner's group */

struct pmsg { long mtype; char mtext[4]; };

static int qid = -1;
static int pipefd[2];

/* Child, as the creator: make a queue, give it to the new owner, report the identifier. */
static int create_and_give(void *p)
{
	struct msqid_ds ds;
	int id;

	(void)p;
	id = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0600);
	if (id < 0)
		return R_OTHER;
	if (msgctl(id, IPC_STAT, &ds) != 0)
		return R_OTHER;
	ds.msg_perm.uid = O_U;
	ds.msg_perm.gid = O_G;
	if (msgctl(id, IPC_SET, &ds) != 0) {
		(void)msgctl(id, IPC_RMID, NULL);
		return R_OTHER;
	}
	if (write(pipefd[1], &id, sizeof id) != (ssize_t)sizeof id)
		return R_OTHER;
	return R_ALLOWED;
}

/* Child: may this process read the queue? */
static int try_read(void *p)
{
	int id = *(int *)p;
	struct pmsg r;
	int ok = (msgrcv(id, &r, 1, 0, IPC_NOWAIT | MSG_NOERROR) >= 0);

	return (ok || errno == ENOMSG) ? R_ALLOWED : verdict(0, errno);
}

static void cleanup(void)
{
	if (qid >= 0)
		(void)msgctl(qid, IPC_RMID, NULL);
}

static void set_mode(unsigned mode)
{
	struct msqid_ds ds;

	NEED(msgctl(qid, IPC_STAT, &ds) == 0, "IPC_STAT failed: %s", pc_errname(errno));
	ds.msg_perm.mode = mode;
	NEED(msgctl(qid, IPC_SET, &ds) == 0, "IPC_SET failed: %s", pc_errname(errno));
}

struct role { uid_t u; gid_t g; int user_mode_ok; int group_mode_ok; const char *what; };

int main(void)
{
	static const struct role roles[] = {
		/* mode 0600 (user class), mode 0060 (group class) */
		{ C_U, ID_Y, 1, 0, "the creator's user ID only" },
		{ O_U, ID_Y, 1, 0, "the owner's user ID only" },
		{ ID_X, ID_Y, 0, 0, "no ID of the queue" },
		{ ID_X, C_G, 0, 1, "the creator's group ID only" },
		{ ID_X, O_G, 0, 1, "the owner's group ID only" },
		{ C_U, O_G, 1, 0, "the creator's user ID and the owner's group ID" },
		{ O_U, C_G, 1, 0, "the owner's user ID and the creator's group ID" },
	};
	size_t i;
	int v, rc;

	pc_begin("XSH.2.7-xsi-ipc.005");
	NEED(geteuid() == 0, "the test needs root to change user and group IDs");
	NEED(pipe(pipefd) == 0, "pipe failed");
	(void)signal(SIGALRM, ipc_on_alarm);
	(void)atexit(cleanup);
	pc_test();
	(void)alarm(40);
	rc = run_as(C_U, C_G, create_and_give, NULL);
	NEED(rc == R_ALLOWED, "the creator child failed: %s", verdict_name(rc));
	NEED(read(pipefd[0], &qid, sizeof qid) == (ssize_t)sizeof qid && qid >= 0, "no queue identifier came back");
	for (i = 0; i < sizeof roles / sizeof roles[0]; i++) {
		set_mode(0600);
		v = run_as(roles[i].u, roles[i].g, try_read, &qid);
		NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
		pc_check(v == (roles[i].user_mode_ok ? R_ALLOWED : R_DENIED),
		    "mode 0600, process with %s: msgrcv() was %s", roles[i].what, verdict_name(v));
		set_mode(0060);
		v = run_as(roles[i].u, roles[i].g, try_read, &qid);
		NEED(v >= 0 && v <= 2, "child could not run: %s", verdict_name(v));
		pc_check(v == (roles[i].group_mode_ok ? R_ALLOWED : R_DENIED),
		    "mode 0060, process with %s: msgrcv() was %s", roles[i].what, verdict_name(v));
	}
	pc_pass();
}
