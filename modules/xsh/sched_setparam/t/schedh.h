/*
 * schedh.h - helpers shared by the scheduling tests (copied unchanged into
 * each module that uses them).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Changes of policy are made only in child processes, which exit when they
 * are done, so no later test is affected. A child never calls the pc_*
 * result functions: it returns a code and a message, and the parent
 * reports. Every wait is bounded (alarm in the child, poll in the parent).
 */
#ifndef SCHEDH_H
#define SCHEDH_H

#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SH_PASS 0
#define SH_FAIL 1
#define SH_UNRES 2

#define SH_SAY(msg, ...) snprintf((msg), 200, __VA_ARGS__)

typedef int (*sh_body)(char *msg);

/* A policy value that is none of the policies <sched.h> defines. */
static inline int sh_bad_policy(void)
{
	int v = INT_MAX;

	for (;;) {
		if (v != SCHED_FIFO && v != SCHED_RR && v != SCHED_OTHER
#ifdef SCHED_SPORADIC
		    && v != SCHED_SPORADIC
#endif
		    )
			return v;
		v--;
	}
}

/* Range of a policy; UNRES unless it is usable for out-of-range tests. */
static inline int sh_rng(int pol, int *lo, int *hi, char *msg)
{
	errno = 0;
	*lo = sched_get_priority_min(pol);
	if (*lo == -1 && errno != 0) {
		SH_SAY(msg, "sched_get_priority_min(%d) failed: %s", pol, pc_errname(errno));
		return SH_UNRES;
	}
	errno = 0;
	*hi = sched_get_priority_max(pol);
	if (*hi == -1 && errno != 0) {
		SH_SAY(msg, "sched_get_priority_max(%d) failed: %s", pol, pc_errname(errno));
		return SH_UNRES;
	}
	if (*lo <= INT_MIN || *hi >= INT_MAX || *hi - *lo < 3) {
		SH_SAY(msg, "range of policy %d (%d..%d) is unusable here", pol, *lo, *hi);
		return SH_UNRES;
	}
	return SH_PASS;
}

/* Setup in a child: make the calling process use (pol, prio). */
static inline int sh_enter(int pol, int prio, char *msg)
{
	struct sched_param sp;

	memset(&sp, 0, sizeof sp);
	sp.sched_priority = prio;
	if (sched_setscheduler(0, pol, &sp) == -1) {
		SH_SAY(msg, "cannot set policy %d priority %d: %s", pol, prio, pc_errname(errno));
		return SH_UNRES;
	}
	return SH_PASS;
}

/* The calling process must have policy pol and priority prio. */
static inline int sh_expect(int pol, int prio, char *msg)
{
	struct sched_param sp;
	int p;

	memset(&sp, 0, sizeof sp);
	p = sched_getscheduler(0);
	if (p != pol) {
		SH_SAY(msg, "policy is %d, expected %d", p, pol);
		return SH_FAIL;
	}
	if (sched_getparam(0, &sp) != 0) {
		SH_SAY(msg, "sched_getparam failed: %s", pc_errname(errno));
		return SH_FAIL;
	}
	if (sp.sched_priority != prio) {
		SH_SAY(msg, "priority is %d, expected %d", sp.sched_priority, prio);
		return SH_FAIL;
	}
	return SH_PASS;
}

/* Give up privilege (when there is any); non-zero if that is not certain. */
static inline int sh_drop(char *msg)
{
	if (geteuid() != 0)
		return 0;
	(void)pc_drop_groups();
	if (setgid(65534) != 0 || setuid(65534) != 0) {
		SH_SAY(msg, "cannot drop privilege: %s", pc_errname(errno));
		return 1;
	}
	if (geteuid() == 0 || setuid(0) == 0) {
		SH_SAY(msg, "privilege is not gone");
		return 1;
	}
	return 0;
}

static inline void sh_send(int fd, int code, const char *msg)
{
	char buf[256];
	ssize_t r;

	buf[0] = (char)code;
	snprintf(buf + 1, sizeof buf - 1, "%s", msg);
	r = write(fd, buf, strlen(buf));
	(void)r;
}

/* Read one message of at most 10 s; returns its length, 0 on EOF or timeout. */
static inline int sh_recv(int fd, char *buf, size_t n)
{
	struct pollfd pf;
	ssize_t r;
	int k;

	pf.fd = fd;
	pf.events = POLLIN;
	do {
		k = poll(&pf, 1, 10000);
	} while (k < 0 && errno == EINTR);
	if (k <= 0)
		return 0;
	do {
		r = read(fd, buf, n - 1);
	} while (r < 0 && errno == EINTR);
	if (r <= 0)
		return 0;
	buf[r] = 0;
	return (int)r;
}

static inline void sh_reap(pid_t p)
{
	int st;

	while (waitpid(p, &st, 0) < 0 && errno == EINTR)
		;
}

/*
 * Run body in a child (privilege dropped first when drop is set) and report
 * its verdict. A child that dies or hangs is a FAIL: it was running the
 * calls under test.
 */
static inline void sh_run(sh_body body, int drop)
{
	int fd[2], st = 0, r;
	pid_t p;
	char buf[256];

	if (pipe(fd) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	p = fork();
	if (p < 0)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (p == 0) {
		char msg[200];

		close(fd[0]);
		alarm(30);
		msg[0] = 0;
		if (drop && sh_drop(msg))
			r = SH_UNRES;
		else
			r = body(msg);
		sh_send(fd[1], '0' + r, msg);
		_exit(0);
	}
	close(fd[1]);
	buf[0] = 0;
	r = sh_recv(fd[0], buf, sizeof buf);
	close(fd[0]);
	if (r == 0) {
		(void)kill(p, SIGKILL);
		sh_reap(p);
		pc_fail("the child hung or died without a verdict");
	}
	while (waitpid(p, &st, 0) < 0 && errno == EINTR)
		;
	if (buf[0] == '0' + SH_UNRES)
		pc_unresolved("%s", buf + 1);
	if (buf[0] != '0' + SH_PASS)
		pc_fail("%s", buf + 1);
}

/* A pid that no process has: a child already reaped. */
static inline pid_t sh_dead_pid(void)
{
	pid_t p = fork();

	if (p < 0)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (p == 0)
		_exit(0);
	sh_reap(p);
	return p;
}

/*
 * A peer process held in a chosen state: the child enters (ipol, iprio)
 * (when ipol >= 0), reports ready, waits for a release byte, then checks
 * it has (epol, eprio) (when epol >= 0) and reports.
 */
struct sh_peer {
	pid_t pid;
	int w, r;
	int ok;
};

static inline void sh_peer_start(struct sh_peer *pe, int ipol, int iprio, int epol, int eprio)
{
	int a[2], b[2];
	char msg[200], buf[256];
	ssize_t n;

	if (pipe(a) != 0 || pipe(b) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	pe->pid = fork();
	if (pe->pid < 0)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pe->pid == 0) {
		char c;
		int r = SH_PASS;

		close(a[1]);
		close(b[0]);
		alarm(30);
		msg[0] = 0;
		if (ipol >= 0 && sh_enter(ipol, iprio, msg) != SH_PASS) {
			sh_send(b[1], 'X', msg);
			_exit(0);
		}
		sh_send(b[1], 'R', "");
		while ((n = read(a[0], &c, 1)) < 0 && errno == EINTR)
			;
		if (epol >= 0)
			r = sh_expect(epol, eprio, msg);
		sh_send(b[1], r == SH_PASS ? 'P' : 'F', msg);
		_exit(0);
	}
	close(a[0]);
	close(b[1]);
	pe->w = a[1];
	pe->r = b[0];
	if (sh_recv(pe->r, buf, sizeof buf) == 0 || buf[0] != 'R') {
		const char *why = buf[0] == 'X' ? buf + 1 : "the peer did not become ready";

		(void)kill(pe->pid, SIGKILL);
		sh_reap(pe->pid);
		pc_unresolved("%s", why);
	}
}

/* Release the peer and collect its verdict (SH_PASS or SH_FAIL, msg set). */
static inline int sh_peer_finish(struct sh_peer *pe, char *msg)
{
	char buf[256];
	ssize_t r;
	int code;

	r = write(pe->w, "g", 1);
	(void)r;
	if (sh_recv(pe->r, buf, sizeof buf) == 0) {
		(void)kill(pe->pid, SIGKILL);
		sh_reap(pe->pid);
		SH_SAY(msg, "the peer gave no verdict");
		return SH_FAIL;
	}
	sh_reap(pe->pid);
	close(pe->w);
	close(pe->r);
	code = buf[0] == 'P' ? SH_PASS : SH_FAIL;
	if (code != SH_PASS)
		SH_SAY(msg, "peer: %s", buf + 1);
	return code;
}

/* Thread forms of the above. */
static inline int sh_tget(pthread_t t, int *pol, int *prio)
{
	struct sched_param sp;
	int e;

	memset(&sp, 0, sizeof sp);
	sp.sched_priority = INT_MIN;
	*pol = -1;
	e = pthread_getschedparam(t, pol, &sp);
	*prio = sp.sched_priority;
	return e;
}

static inline int sh_tset(pthread_t t, int pol, int prio)
{
	struct sched_param sp;

	memset(&sp, 0, sizeof sp);
	sp.sched_priority = prio;
	return pthread_setschedparam(t, pol, &sp);
}

/* Thread t must have policy pol and priority prio. */
static inline int sh_texpect(pthread_t t, int pol, int prio, char *msg)
{
	int p, q, e;

	e = sh_tget(t, &p, &q);
	if (e != 0) {
		SH_SAY(msg, "pthread_getschedparam failed: %s", pc_errname(e));
		return SH_FAIL;
	}
	if (p != pol || q != prio) {
		SH_SAY(msg, "thread has policy %d priority %d, expected %d and %d", p, q, pol, prio);
		return SH_FAIL;
	}
	return SH_PASS;
}

/* Setup: put thread t under (pol, prio); UNRES when the system refuses. */
static inline int sh_tenter(pthread_t t, int pol, int prio, char *msg)
{
	int e = sh_tset(t, pol, prio);

	if (e != 0) {
		SH_SAY(msg, "cannot set policy %d priority %d on the thread: %s", pol, prio, pc_errname(e));
		return SH_UNRES;
	}
	return SH_PASS;
}

#endif
