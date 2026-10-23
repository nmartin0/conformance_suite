/*
 * A child does not inherit its parent's locks.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.032
 * MUTANT    fcntl.setlk_conflict_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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
 * Another process asks F_GETLK about *q on "f" and the answer comes back
 * in *q. Failures of the probe itself are UNRESOLVED.
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
		if (fd < 0 || fcntl(fd, F_GETLK, &a) != 0)
			_exit(2);
		if (write(p[1], &a, sizeof a) != (ssize_t)sizeof a)
			_exit(2);
		_exit(0);
	}
	(void)close(p[1]);
	if (waitpid(pid, &st, 0) != pid || !WIFEXITED(st) || WEXITSTATUS(st) != 0)
		pc_unresolved("probe: F_GETLK in the other process failed");
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

static int hold_ctl = -1;
static pid_t hold_pid;

/*
 * Another process takes a lock on "f" and keeps it until release_holder()
 * (secs 0) or for secs seconds. Call it in the setup phase.
 */
static void holder(int type, off_t start, off_t len, unsigned secs)
{
	int sp[2], cp[2];
	char c;

	if (pipe(sp) != 0 || pipe(cp) != 0)
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
		if (secs)
			sleep(secs);
		else
			(void)read(cp[0], &c, 1);	/* until the parent lets go */
		_exit(0);
	}
	(void)close(sp[1]);
	(void)close(cp[0]);
	hold_ctl = cp[1];
	if (read(sp[0], &c, 1) != 1 || c != 'y')
		pc_unresolved("holder: could not take its lock");
	(void)close(sp[0]);
}

static void release_holder(void)
{
	(void)close(hold_ctl);
	(void)waitpid(hold_pid, NULL, 0);
}

int main(void)
{
	struct flock fl;
	int fd, st;
	pid_t pid;

	pc_begin("XSH.fcntl.032");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	fl = lk(F_WRLCK, SEEK_SET, 0, 10);
	pc_check(fcntl(fd, F_SETLK, &fl) == 0, "F_SETLK failed");
	pc_test();
	pid = fork();
	if (pid == -1)
		pc_unresolved("fork failed");
	if (pid == 0) {
		struct flock c = lk(F_WRLCK, SEEK_SET, 0, 10);
		int r = fcntl(fd, F_SETLK, &c);
		_exit(r == -1 && (errno == EACCES || errno == EAGAIN) ? 0 : 1);
	}
	if (waitpid(pid, &st, 0) != pid || !WIFEXITED(st))
		pc_unresolved("child did not finish");
	pc_check(WEXITSTATUS(st) == 0, "the child could lock bytes its parent holds");
	pc_pass();
}
