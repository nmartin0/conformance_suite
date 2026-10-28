/*
 * A thread blocked in waitpid() or in fcntl(F_SETLKW) does not keep the
 * thread that would unblock it from running.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.2-concurrent-execution.004
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

static const char *names[2] = { "waitpid", "fcntl(F_SETLKW)" };
static int which;
static volatile int blocked_done;
static pid_t kid;
static int cfd[2];
static const char *lockfile = "lock.4.2";

static void on_alarm(int s)
{
	(void)s;
	pc_fail("timed out: a thread blocked in %s kept the releasing thread from running", names[which]);
}

static void *blocker(void *arg)
{
	int st, fd;
	struct flock fl;

	(void)arg;
	if (which == 0) {
		while (waitpid(kid, &st, 0) == -1 && errno == EINTR)
			;
		blocked_done = 1;
	} else {
		fd = open(lockfile, O_RDWR);
		if (fd >= 0) {
			fl.l_type = F_WRLCK;
			fl.l_whence = SEEK_SET;
			fl.l_start = 0;
			fl.l_len = 0;
			if (fcntl(fd, F_SETLKW, &fl) == 0)
				blocked_done = 1;
			(void)close(fd);
		}
	}
	return NULL;
}

static void *releaser(void *arg)
{
	struct timespec nap = { 0, 150000000L };

	(void)arg;
	(void)nanosleep(&nap, NULL);	/* let the other thread block first */
	(void)write(cfd[1], "x", 1);	/* the child then ends or unlocks */
	return NULL;
}

int main(void)
{
	pthread_t tb, tr;
	int k, r, st, lfd;
	char c;
	struct flock fl;

	pc_begin("XBD.4.2-concurrent-execution.004");
	(void)signal(SIGALRM, on_alarm);
	(void)unlink(lockfile);
	pc_make_file(lockfile, "x");
	pc_test();
	(void)alarm(60);
	for (k = 0; k < 2; k++) {
		which = k;
		blocked_done = 0;
		if (pipe(cfd) != 0)
			pc_unresolved("pipe failed: %s", pc_errname(errno));
		if (k == 1) {
			/* the child holds the lock until it has read a byte; wait until it does */
			int ready[2];

			if (pipe(ready) != 0)
				pc_unresolved("pipe failed: %s", pc_errname(errno));
			kid = fork();
			if (kid == -1)
				pc_unresolved("fork failed: %s", pc_errname(errno));
			if (kid == 0) {
				lfd = open(lockfile, O_RDWR);
				fl.l_type = F_WRLCK;
				fl.l_whence = SEEK_SET;
				fl.l_start = 0;
				fl.l_len = 0;
				if (lfd < 0 || fcntl(lfd, F_SETLK, &fl) != 0)
					_exit(2);
				(void)write(ready[1], "r", 1);
				_exit(read(cfd[0], &c, 1) == 1 ? 0 : 3);
			}
			(void)close(ready[1]);
			if (read(ready[0], &c, 1) != 1)
				pc_unresolved("the child did not take the lock");
			(void)close(ready[0]);
		} else {
			kid = fork();
			if (kid == -1)
				pc_unresolved("fork failed: %s", pc_errname(errno));
			if (kid == 0)
				_exit(read(cfd[0], &c, 1) == 1 ? 0 : 3);
		}
		r = pthread_create(&tb, NULL, blocker, NULL);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		r = pthread_create(&tr, NULL, releaser, NULL);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		(void)pthread_join(tr, NULL);
		(void)pthread_join(tb, NULL);
		pc_check(blocked_done, "the thread blocked in %s did not complete", names[k]);
		if (k == 1)
			(void)waitpid(kid, &st, 0);
		(void)close(cfd[0]);
		(void)close(cfd[1]);
	}
	(void)unlink(lockfile);
	pc_pass();
}
