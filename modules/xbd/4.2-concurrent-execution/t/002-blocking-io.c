/*
 * A thread blocked in read() (pipe, socket pair), poll(), a FIFO open,
 * accept() or sigwait() does not keep the thread that would unblock it
 * from running.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.2-concurrent-execution.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#define NCASES 6

static const char *names[NCASES] = { "read(pipe)", "read(socketpair)", "poll", "open(FIFO)", "accept", "sigwait" };
static int which;
static volatile int blocked_done;
static int pfd[2], sv[2], lfd = -1;
static sigset_t sset;
static const char *fifo = "fifo.4.2";
static const char *sock = "sock.4.2";

static void on_alarm(int s)
{
	(void)s;
	pc_fail("timed out: a thread blocked in %s kept the releasing thread from running", names[which]);
}

static void *blocker(void *arg)
{
	char c;
	int fd, sig;
	struct pollfd p;

	(void)arg;
	switch (which) {
	case 0:
		if (read(pfd[0], &c, 1) == 1)
			blocked_done = 1;
		break;
	case 1:
		if (read(sv[0], &c, 1) == 1)
			blocked_done = 1;
		break;
	case 2:
		p.fd = pfd[0];
		p.events = POLLIN;
		p.revents = 0;
		if (poll(&p, 1, -1) == 1 && (p.revents & POLLIN))
			blocked_done = 1;
		break;
	case 3:
		fd = open(fifo, O_RDONLY);
		if (fd >= 0) {
			(void)close(fd);
			blocked_done = 1;
		}
		break;
	case 4:
		fd = accept(lfd, NULL, NULL);
		if (fd >= 0) {
			(void)close(fd);
			blocked_done = 1;
		}
		break;
	default:
		if (sigwait(&sset, &sig) == 0 && sig == SIGUSR1)
			blocked_done = 1;
		break;
	}
	return NULL;
}

static void *releaser(void *arg)
{
	struct timespec nap = { 0, 100000000L };
	int fd;
	struct sockaddr_un a;

	(void)arg;
	(void)nanosleep(&nap, NULL);	/* let the other thread block first */
	switch (which) {
	case 0:
	case 2:
		(void)write(pfd[1], "x", 1);
		break;
	case 1:
		(void)write(sv[1], "x", 1);
		break;
	case 3:
		fd = open(fifo, O_WRONLY);
		if (fd >= 0)
			(void)close(fd);
		break;
	case 4:
		fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (fd >= 0) {
			memset(&a, 0, sizeof a);
			a.sun_family = AF_UNIX;
			strcpy(a.sun_path, sock);
			(void)connect(fd, (struct sockaddr *)&a, sizeof a);
			(void)close(fd);
		}
		break;
	default:
		(void)kill(getpid(), SIGUSR1);
		break;
	}
	return NULL;
}

int main(void)
{
	pthread_t tb, tr;
	struct sockaddr_un a;
	int k, r;

	pc_begin("XBD.4.2-concurrent-execution.002");
	(void)signal(SIGALRM, on_alarm);
	(void)unlink(fifo);
	(void)unlink(sock);
	if (mkfifo(fifo, 0600) != 0)
		pc_unresolved("mkfifo failed: %s", pc_errname(errno));
	lfd = socket(AF_UNIX, SOCK_STREAM, 0);
	memset(&a, 0, sizeof a);
	a.sun_family = AF_UNIX;
	strcpy(a.sun_path, sock);
	if (lfd < 0 || bind(lfd, (struct sockaddr *)&a, sizeof a) != 0 || listen(lfd, 1) != 0)
		pc_unresolved("cannot set up a listening socket: %s", pc_errname(errno));
	sigemptyset(&sset);
	sigaddset(&sset, SIGUSR1);
	if (pthread_sigmask(SIG_BLOCK, &sset, NULL) != 0)
		pc_unresolved("pthread_sigmask failed");
	pc_test();
	(void)alarm(60);
	for (k = 0; k < NCASES; k++) {
		which = k;
		blocked_done = 0;
		if (pipe(pfd) != 0 || socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0)
			pc_unresolved("pipe or socketpair failed: %s", pc_errname(errno));
		r = pthread_create(&tb, NULL, blocker, NULL);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		r = pthread_create(&tr, NULL, releaser, NULL);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		(void)pthread_join(tr, NULL);
		(void)pthread_join(tb, NULL);
		pc_check(blocked_done, "the thread blocked in %s did not complete", names[k]);
		(void)close(pfd[0]);
		(void)close(pfd[1]);
		(void)close(sv[0]);
		(void)close(sv[1]);
	}
	(void)close(lfd);
	(void)unlink(fifo);
	(void)unlink(sock);
	pc_pass();
}
