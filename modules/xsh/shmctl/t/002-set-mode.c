/*
 * IPC_SET changes the mode bits and sets ctime.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.shmctl.002
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

#include <sys/shm.h>

static int ids[16];
static int nids;

static inline void cleanup(void)
{
	while (nids > 0)
		(void)shmctl(ids[--nids], IPC_RMID, NULL);
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
		pc_notinuse("XSI shared memory is not provided (ENOSYS)");
}

/* shmget() as the call under test: ENOSYS is NOTINUSE. */
static inline int sg(key_t key, size_t size, int flg)
{
	int id = shmget(key, size, flg);

	if (id < 0)
		check_enosys();
	return id;
}

/* A new private segment (setup use). */
static inline int sh(size_t size, int mode)
{
	int id = shmget(IPC_PRIVATE, size, mode);

	if (id < 0)
		check_enosys();
	NEED(id >= 0, "shmget(IPC_PRIVATE) failed: %s", pc_errname(errno));
	return track(id);
}

/* Attach read-write at a system-chosen address (setup use). */
static inline char *at(int id)
{
	void *p = shmat(id, NULL, 0);

	NEED(p != SHM_FAILED, "shmat failed: %s", pc_errname(errno));
	return p;
}

static inline struct shmid_ds st(int id)
{
	struct shmid_ds d;

	NEED(shmctl(id, IPC_STAT, &d) == 0, "shmctl(IPC_STAT) failed: %s", pc_errname(errno));
	return d;
}

static inline void set_mode(int id, int mode)
{
	struct shmid_ds d = st(id);

	d.shm_perm.mode = (mode_t)mode;
	NEED(shmctl(id, IPC_SET, &d) == 0, "shmctl(IPC_SET) failed: %s", pc_errname(errno));
}


int main(void)
{
	pc_begin("XSH.shmctl.002");

	int id = sh(4096, 0640);
	struct shmid_ds d, e;
	time_t t0, t1;

	d = st(id);
	d.shm_perm.mode = 0600;
	pc_test();
	t0 = time(NULL);
	pc_check(shmctl(id, IPC_SET, &d) == 0, "shmctl(IPC_SET) failed: %s", pc_errname(errno));
	t1 = time(NULL);
	pc_check(shmctl(id, IPC_STAT, &e) == 0, "shmctl(IPC_STAT) failed: %s", pc_errname(errno));
	pc_check((e.shm_perm.mode & 0777) == 0600, "mode bits %#o, expected 0600", (unsigned)(e.shm_perm.mode & 0777));
	in_range((long)e.shm_ctime, (long)t0, (long)t1, "shm_ctime");
	pc_pass();
}
