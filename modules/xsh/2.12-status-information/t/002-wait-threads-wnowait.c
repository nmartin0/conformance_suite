/*
 * Threads waiting for the same child: at least one gets the status, and not both plain waiters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.12-status-information.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static pid_t child;
static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
static volatile int done[3], got[3], statv[3];

static void on_alarm(int s)
{
	(void)s;
	if (child > 0) {
		(void)kill(child, SIGKILL);
		(void)waitpid(child, NULL, 0);
	}
	pc_unresolved("the test timed out");
}

static void *plain(void *arg)
{
	int k = (int)(long)arg, st = 0;
	pid_t r = waitpid(child, &st, 0);

	pthread_mutex_lock(&mu);
	got[k] = (r == child);
	statv[k] = st;
	done[k] = 1;
	pthread_mutex_unlock(&mu);
	return NULL;
}

static void *nowait(void *arg)
{
	siginfo_t si;
	int r;

	(void)arg;
	si.si_pid = 0;
	r = waitid(P_PID, (id_t)child, &si, WEXITED | WNOWAIT);
	pthread_mutex_lock(&mu);
	got[2] = (r == 0 && si.si_pid == child);
	statv[2] = si.si_status;
	done[2] = 1;
	pthread_mutex_unlock(&mu);
	return NULL;
}

int main(void)
{
	pthread_t t[3];
	int fds[2], i, n, plains, total, st;
	struct timespec ts = { 0, 300000000 };

	pc_begin("XSH.2.12-status-information.002");
	(void)signal(SIGALRM, on_alarm);
	(void)alarm(25);
	NEED(pipe(fds) == 0, "pipe failed");
	child = fork();
	NEED(child != -1, "fork failed");
	if (child == 0) {
		char c;

		(void)close(fds[1]);
		(void)read(fds[0], &c, 1);
		_exit(3);
	}
	NEED(pthread_create(&t[0], NULL, plain, (void *)0L) == 0, "pthread_create failed");
	NEED(pthread_create(&t[1], NULL, plain, (void *)1L) == 0, "pthread_create failed");
	NEED(pthread_create(&t[2], NULL, nowait, NULL) == 0, "pthread_create failed");
	(void)nanosleep(&ts, NULL);
	pc_test();
	NEED(write(fds[1], "x", 1) == 1, "write failed");
	for (i = 0; i < 300; i++) {
		ts.tv_sec = 0;
		ts.tv_nsec = 10000000;
		pthread_mutex_lock(&mu);
		n = done[0] + done[1] + done[2];
		pthread_mutex_unlock(&mu);
		if (n == 3)
			break;
		(void)nanosleep(&ts, NULL);
	}
	/* Whatever is still blocked is cancelled before it is joined. */
	for (i = 0; i < 3; i++) {
		pthread_mutex_lock(&mu);
		n = done[i];
		pthread_mutex_unlock(&mu);
		if (!n)
			(void)pthread_cancel(t[i]);
		(void)pthread_join(t[i], NULL);
	}
	plains = got[0] + got[1];
	total = plains + got[2];
	if (waitpid(child, &st, WNOHANG) < 0)
		st = 0;
	pc_check(total >= 1, "none of the three threads obtained the status of the exited child");
	pc_check(plains <= 1, "both threads in waitpid() obtained the status of the one child");
	for (i = 0; i < 3; i++) {
		if (got[i])
			pc_check(i == 2 ? statv[i] == 3 : (WIFEXITED(statv[i]) && WEXITSTATUS(statv[i]) == 3),
			    "thread %d obtained a status that is not the exit with 3 (%d)", i, statv[i]);
	}
	pc_pass();
}
