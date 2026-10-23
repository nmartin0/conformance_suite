/*
 * F_OFD_GETLK reports as F_GETLK does, for OFD queries.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.038
 * MUTANT    fcntl.ofd_getlk_pid_wrong
 * MUTANT    fcntl.getlk_clobbers
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef F_OFD_SETLK
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
#endif

int main(void)
{
#ifdef F_OFD_SETLK
	struct flock q;
	int fd;

#endif
	pc_begin("XSH.fcntl.038");
#ifndef F_OFD_SETLK
	pc_unresolved("<fcntl.h> does not define F_OFD_SETLK");
#else
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	holder(F_WRLCK, 5, 2, 0);
	pc_test();
	q = lk(F_WRLCK, SEEK_SET, 0, 5);
	pc_expect_ok(fcntl(fd, F_OFD_GETLK, &q), "F_OFD_GETLK on free bytes");
	pc_check(q.l_type == F_UNLCK && q.l_whence == SEEK_SET && q.l_start == 0 &&
	    q.l_len == 5 && q.l_pid == 0,
	    "F_OFD_GETLK on free bytes changed more than l_type");
	q = lk(F_WRLCK, SEEK_SET, 0, 0);
	pc_expect_ok(fcntl(fd, F_OFD_GETLK, &q), "F_OFD_GETLK on held bytes");
	pc_check(q.l_type == F_WRLCK && q.l_start == 5 && q.l_len == 2,
	    "reported type %d start %ld len %ld, expected the holder's [5,7)",
	    q.l_type, (long)q.l_start, (long)q.l_len);
	pc_check(q.l_pid == hold_pid, "l_pid %ld, expected the holder %ld",
	    (long)q.l_pid, (long)hold_pid);
	release_holder();
	pc_pass();
#endif
}
