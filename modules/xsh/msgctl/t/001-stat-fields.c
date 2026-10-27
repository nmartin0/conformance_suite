/*
 * IPC_STAT reports queue contents and ownership.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.msgctl.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static inline void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out (a call did not return)");
}

/* Limit the whole test: a hang becomes a FAIL (cleanup runs at exit). */
static inline void guard(unsigned secs)
{
	(void)signal(SIGALRM, on_alarm);
	(void)alarm(secs);
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Run fn in a child; its exit status is what fn returns (0 to 96). */
static inline pid_t spawn(int (*fn)(void))
{
	pid_t k = fork();

	if (k == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (k == 0) {
		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(25);
		_exit(fn());
	}
	return k;
}

/* Exit status of k; -1 if it did not end within secs (it is killed), -2 if a signal ended it. */
static inline int reap(pid_t k, int secs)
{
	int st, i;

	for (i = 0; i < secs * 100; i++) {
		pid_t r = waitpid(k, &st, WNOHANG);

		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -2;
		if (r == -1 && errno != EINTR)
			return -2;
		sleep_ms(10);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

/* Whether k has not ended yet (its status stays collectable). */
static inline int running(pid_t k)
{
	siginfo_t si;

	si.si_pid = 0;
	if (waitid(P_PID, (id_t)k, &si, WEXITED | WNOHANG | WNOWAIT) != 0)
		pc_unresolved("waitid failed: %s", pc_errname(errno));
	return si.si_pid == 0;
}

/*
 * Child result for "the call must fail with err": 0 when it did, 1 when it
 * succeeded, 100 + errno for another error.
 */
static inline int want_err(long r, int err)
{
	if (r != -1)
		return 1;
	return errno == err ? 0 : 100 + (errno < 150 ? errno : 149);
}

static inline const char *child_said(int rc)
{
	static char buf[64];

	if (rc == 0)
		return "as expected";
	if (rc == 1)
		return "the call succeeded";
	if (rc >= 100) {
		snprintf(buf, sizeof buf, "failed with %s", pc_errname(rc - 100));
		return buf;
	}
	if (rc < 0)
		return "the child did not end normally or in time";
	snprintf(buf, sizeof buf, "code %d", rc);
	return buf;
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534. Returns what fn returns
 * (0 to 96); a child that cannot drop privileges makes the test UNRESOLVED.
 */
static inline int as_other(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(25);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static inline void in_range(long v, long lo, long hi, const char *what)
{
	pc_check(v >= lo && v <= hi, "%s is %ld, expected between %ld and %ld", what, v, lo, hi);
}

/* A handler that does nothing, installed without SA_RESTART. */
static inline void noop_handler(int s)
{
	(void)s;
}

static inline void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = noop_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed: %s", pc_errname(errno));
}

/* Send SIGUSR1 to k every 100 ms until it ends; its exit status as reap(). */
static inline int interrupt_until_done(pid_t k, int secs)
{
	int i, st;

	for (i = 0; i < secs * 10; i++) {
		pid_t r;

		sleep_ms(100);
		r = waitpid(k, &st, WNOHANG);
		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -2;
		(void)kill(k, SIGUSR1);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

/* A key from ftok() on a fresh file, id 'q' unless given. */
static inline key_t fresh_key(const char *file, int id)
{
	key_t k;

	pc_make_file(file, "");
	k = ftok(file, id);
	if (k == (key_t)-1)
		pc_unresolved("ftok(%s) failed: %s", file, pc_errname(errno));
	return k;
}

#include <sys/msg.h>

static int ids[16];
static int nids;

static inline void cleanup(void)
{
	while (nids > 0)
		(void)msgctl(ids[--nids], IPC_RMID, NULL);
}

static inline int track(int id)
{
	if (nids == 0)
		atexit(cleanup);
	if (nids < 16)
		ids[nids++] = id;
	return id;
}

static inline void check_enosys(void)
{
	if (errno == ENOSYS)
		pc_notinuse("XSI message queues are not provided (ENOSYS)");
}

/* msgget() as the call under test: ENOSYS is NOTINUSE. */
static inline int mg(key_t key, int flg)
{
	int id = msgget(key, flg);

	if (id < 0)
		check_enosys();
	return id;
}

/* A new private queue (setup use). */
static inline int q(int mode)
{
	int id = msgget(IPC_PRIVATE, mode);

	if (id < 0)
		check_enosys();
	NEED(id >= 0, "msgget(IPC_PRIVATE) failed: %s", pc_errname(errno));
	return track(id);
}

struct mb {
	long mtype;
	char mtext[64];
};

static inline int snd(int id, long t, const char *s, int flg)
{
	struct mb m;
	size_t n = strlen(s);

	m.mtype = t;
	memcpy(m.mtext, s, n);
	return msgsnd(id, &m, n, flg);
}

static inline struct msqid_ds st(int id)
{
	struct msqid_ds d;

	NEED(msgctl(id, IPC_STAT, &d) == 0, "msgctl(IPC_STAT) failed: %s", pc_errname(errno));
	return d;
}

static inline void set_mode(int id, int mode)
{
	struct msqid_ds d = st(id);

	d.msg_perm.mode = (mode_t)mode;
	NEED(msgctl(id, IPC_SET, &d) == 0, "msgctl(IPC_SET) failed: %s", pc_errname(errno));
}

/* Lower msg_qbytes to 256 if that is lower; returns the limit now in force. */
static inline unsigned long small_queue(int id)
{
	struct msqid_ds d = st(id);

	if (d.msg_qbytes > 256) {
		d.msg_qbytes = 256;
		NEED(msgctl(id, IPC_SET, &d) == 0, "cannot lower msg_qbytes: %s", pc_errname(errno));
		d = st(id);
	}
	NEED(d.msg_qbytes > 0 && d.msg_qbytes <= 100000, "msg_qbytes is %lu, unusable here", (unsigned long)d.msg_qbytes);
	return (unsigned long)d.msg_qbytes;
}

/* Fill the queue with one-byte messages of type 1 until a send fails with EAGAIN; the count. */
static inline unsigned long fill(int id, unsigned long limit)
{
	unsigned long n = 0;

	while (n <= limit + 1) {
		if (snd(id, 1, "x", IPC_NOWAIT) == -1) {
			NEED(errno == EAGAIN, "filling the queue failed with %s", pc_errname(errno));
			return n;
		}
		n++;
	}
	pc_unresolved("the queue never filled up");
}


int main(void)
{
	pc_begin("XSH.msgctl.001");

	int id = q(0640);
	struct msqid_ds d;

	NEED(snd(id, 1, "abc", 0) == 0 && snd(id, 2, "def", 0) == 0, "msgsnd failed: %s", pc_errname(errno));
	pc_test();
	pc_check(msgctl(id, IPC_STAT, &d) == 0, "msgctl(IPC_STAT) failed: %s", pc_errname(errno));
	pc_check(d.msg_qnum == 2, "msg_qnum is %lu, expected 2", (unsigned long)d.msg_qnum);
	pc_check(d.msg_lspid == getpid(), "msg_lspid is %ld, expected %ld", (long)d.msg_lspid, (long)getpid());
	pc_check(d.msg_qbytes > 0, "msg_qbytes is 0");
	pc_check(d.msg_perm.uid == geteuid() && d.msg_perm.cuid == geteuid(), "uid %ld cuid %ld, expected %ld", (long)d.msg_perm.uid, (long)d.msg_perm.cuid, (long)geteuid());
	pc_check(d.msg_perm.gid == getegid() && d.msg_perm.cgid == getegid(), "gid %ld cgid %ld, expected %ld", (long)d.msg_perm.gid, (long)d.msg_perm.cgid, (long)getegid());
	pc_pass();
}
