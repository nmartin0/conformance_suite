/*
 * An interrupted F_SETLKW gives EINTR and sets no lock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.023
 * MUTANT    fcntl.setlkw_as_setlk
 * MUTANT    fcntl.eintr_as_eagain
 * MUTANT    fcntl.setlkw_restarts
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static volatile sig_atomic_t fired;
static int rescue_cfd = -1;

/*
 * Re-arms itself; after two alarms it lets the holder go, so an F_SETLKW
 * that wrongly restarted returns (and the test FAILs) instead of hanging.
 */
static void on_alarm(int sig)
{
	(void)sig;
	if (++fired >= 2 && rescue_cfd >= 0) {
		(void)close(rescue_cfd);
		rescue_cfd = -1;
	}
	alarm(1);
}

/* Fill a struct flock. */
static struct flock lk(int type, int whence, off_t start, off_t len)
{
	struct flock fl;

	memset(&fl, 0, sizeof fl);
	fl.l_type = (short)type;
	fl.l_whence = (short)whence;
	fl.l_start = start;
	fl.l_len = len;
	return fl;
}

/*
 * Set when F_GETLK is what the test checks: then an F_GETLK failure in the
 * probe is FAIL; otherwise the probe only observes and it is UNRESOLVED.
 */
static int getlk_under_test;

/*
 * Another process asks F_GETLK about *q on "f" and the answer comes back
 * in *q.
 */
static void probe(struct flock *q)
{
	int p[2], st, fd;
	pid_t pid;

	if (pipe(p) != 0)
		pc_unresolved("probe: pipe failed");
	pid = fork();
	if (pid == -1)
		pc_unresolved("probe: fork failed");
	if (pid == 0) {
		struct flock a = *q;
		fd = open("f", O_RDWR);
		if (fd < 0)
			_exit(2);
		if (fcntl(fd, F_GETLK, &a) != 0)
			_exit(3);
		if (write(p[1], &a, sizeof a) != (ssize_t)sizeof a)
			_exit(2);
		_exit(0);
	}
	(void)close(p[1]);
	if (waitpid(pid, &st, 0) != pid || !WIFEXITED(st))
		pc_unresolved("probe: the other process did not finish");
	if (WEXITSTATUS(st) == 3 && getlk_under_test)
		pc_fail("F_GETLK failed in the other process");
	if (WEXITSTATUS(st) != 0)
		pc_unresolved("probe: the other process could not ask F_GETLK");
	if (read(p[0], q, sizeof *q) != (ssize_t)sizeof *q)
		pc_unresolved("probe: no answer");
	(void)close(p[0]);
}

/* 1 if another process sees a lock conflicting with type on [start, start+len). */
static int blocked(int type, off_t start, off_t len)
{
	struct flock q = lk(type, SEEK_SET, start, len);

	probe(&q);
	return q.l_type != F_UNLCK;
}

static int hold_ctl = -1, hold_done = -1;
static pid_t hold_pid;

/*
 * Another process takes a lock on "f" and keeps it until release_holder()
 * (secs 0) or for secs seconds. Call it in the setup phase.
 */
static void holder(int type, off_t start, off_t len, unsigned secs)
{
	int sp[2], cp[2], dp[2];
	char c;

	if (pipe(sp) != 0 || pipe(cp) != 0 || pipe(dp) != 0)
		pc_unresolved("holder: pipe failed");
	hold_pid = fork();
	if (hold_pid == -1)
		pc_unresolved("holder: fork failed");
	if (hold_pid == 0) {
		struct flock fl = lk(type, SEEK_SET, start, len);
		int fd = open("f", O_RDWR);
		(void)close(sp[0]);
		(void)close(cp[1]);
		c = fd >= 0 && fcntl(fd, F_SETLK, &fl) == 0 ? 'y' : 'n';
		(void)write(sp[1], &c, 1);
		(void)close(dp[0]);
		if (secs) {
			sleep(secs);
			/* Says it is letting go, then releases the lock. */
			fl.l_type = F_UNLCK;
			(void)write(dp[1], "r", 1);
			(void)fcntl(fd, F_SETLK, &fl);
		} else
			(void)read(cp[0], &c, 1);	/* until the parent lets go */
		_exit(0);
	}
	(void)close(sp[1]);
	(void)close(cp[0]);
	(void)close(dp[1]);
	hold_ctl = cp[1];
	hold_done = dp[0];
	if (read(sp[0], &c, 1) != 1 || c != 'y')
		pc_unresolved("holder: could not take its lock");
	(void)close(sp[0]);
}

static void release_holder(void)
{
	(void)close(hold_ctl);
	while (waitpid(hold_pid, NULL, 0) == -1 && errno == EINTR)
		;
}

int main(void)
{
	struct sigaction sa;
	struct flock fl;
	int fd;

	pc_begin("XSH.fcntl.023");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	holder(F_WRLCK, 0, 10, 0);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	{
		sigset_t s;
		sigemptyset(&s);
		sigaddset(&s, SIGALRM);
		pc_check(sigprocmask(SIG_UNBLOCK, &s, NULL) == 0, "cannot unblock SIGALRM");
	}
	pc_test();
	rescue_cfd = hold_ctl;
	alarm(1);	/* re-armed by the handler */
	fl = lk(F_WRLCK, SEEK_SET, 0, 10);
	pc_expect_err(fcntl(fd, F_SETLKW, &fl), EINTR, "F_SETLKW interrupted by SIGALRM");
	rescue_cfd = -1;	/* the holder is released below */
	alarm(0);
	alarm(0);
	release_holder();
	pc_check(!blocked(F_WRLCK, 0, 10), "the interrupted F_SETLKW left a lock");
	pc_pass();
}
