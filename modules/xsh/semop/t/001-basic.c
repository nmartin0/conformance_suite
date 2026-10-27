/*
 * semop() applies +2 then -1, and a wait-for-zero on zero returns at once;
 * sops is not modified.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.semop.001
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

#include <sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

static int ids[16];
static int nids;

static inline void cleanup(void)
{
	union semun u;

	u.val = 0;
	while (nids > 0)
		(void)semctl(ids[--nids], 0, IPC_RMID, u);
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
		pc_notinuse("XSI semaphores are not provided (ENOSYS)");
}

/* semget() as the call under test: ENOSYS is NOTINUSE. */
static inline int sg(key_t key, int n, int flg)
{
	int id = semget(key, n, flg);

	if (id < 0)
		check_enosys();
	return id;
}

/* A new private set of n semaphores (setup use). */
static inline int sq(int n, int mode)
{
	int id = semget(IPC_PRIVATE, n, mode);

	if (id < 0)
		check_enosys();
	NEED(id >= 0, "semget(IPC_PRIVATE) failed: %s", pc_errname(errno));
	return track(id);
}

static inline int gv(int id, int n)
{
	int v = semctl(id, n, GETVAL);

	NEED(v >= 0, "semctl(GETVAL) failed: %s", pc_errname(errno));
	return v;
}

static inline void sv(int id, int n, int v)
{
	union semun u;

	u.val = v;
	NEED(semctl(id, n, SETVAL, u) == 0, "semctl(SETVAL) failed: %s", pc_errname(errno));
}

static inline int op(int id, int num, int o, int flg)
{
	struct sembuf b;

	b.sem_num = (unsigned short)num;
	b.sem_op = (short)o;
	b.sem_flg = (short)flg;
	return semop(id, &b, 1);
}

static inline int cnt(int id, int n, int cmd)
{
	int v = semctl(id, n, cmd);

	NEED(v >= 0, "semctl(count) failed: %s", pc_errname(errno));
	return v;
}

static inline struct semid_ds st(int id)
{
	struct semid_ds d;
	union semun u;

	u.buf = &d;
	NEED(semctl(id, 0, IPC_STAT, u) == 0, "semctl(IPC_STAT) failed: %s", pc_errname(errno));
	return d;
}

static inline void set_mode(int id, int mode)
{
	struct semid_ds d = st(id);
	union semun u;

	d.sem_perm.mode = (mode_t)mode;
	u.buf = &d;
	NEED(semctl(id, 0, IPC_SET, u) == 0, "semctl(IPC_SET) failed: %s", pc_errname(errno));
}


int main(void)
{
	pc_begin("XSH.semop.001");

	int id = sq(2, 0600);
	struct sembuf ops[2], copy[2];

	ops[0].sem_num = 0; ops[0].sem_op = 2; ops[0].sem_flg = 0;
	ops[1].sem_num = 0; ops[1].sem_op = -1; ops[1].sem_flg = 0;
	memcpy(copy, ops, sizeof ops);
	guard(30);
	pc_test();
	pc_check(semop(id, ops, 2) == 0, "semop failed: %s", pc_errname(errno));
	pc_check(memcmp(copy, ops, sizeof ops) == 0, "the sops array was modified");
	pc_check(semctl(id, 0, GETVAL) == 1, "semval is %d, expected 1", semctl(id, 0, GETVAL));
	pc_check(op(id, 1, 0, 0) == 0, "wait-for-zero on a zero semaphore failed: %s", pc_errname(errno));
	pc_pass();
}
