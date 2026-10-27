/*
 * mqh.h - helpers shared by the message queue tests (kept byte-identical in
 * every message queue module's t/ directory).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef MQH_H
#define MQH_H

#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* A step that is not under test: its failure is UNRESOLVED, whatever the phase. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
/* A descriptor value that is never valid. */
#define BADQ ((mqd_t)-1)

/*
 * Queue names for this run: "/pcmq<pid>x<tag>", unique per process. Every
 * name made is removed at exit by the process that made it (children leave
 * with _exit()), and a leftover of an earlier process with this ID is
 * removed first.
 */
static char mq_nm[64][80];
static int mq_nn;
static pid_t mq_owner;

static inline void mq_cleanup(void)
{
	int i;

	if (getpid() != mq_owner)
		return;
	for (i = 0; i < mq_nn; i++)
		(void)mq_unlink(mq_nm[i]);
}

static inline const char *mq_name(const char *tag)
{
	char *n;

	NEED(mq_nn < 64, "too many queue names");
	if (mq_nn == 0) {
		mq_owner = getpid();
		NEED(atexit(mq_cleanup) == 0, "atexit failed");
	}
	n = mq_nm[mq_nn++];
	snprintf(n, sizeof mq_nm[0], "/pcmq%ldx%s", (long)getpid(), tag);
	(void)mq_unlink(n);
	return n;
}

/* A new queue (setup): oflag adds to O_CREAT|O_EXCL; sizes below 1 mean attr NULL. */
static inline mqd_t mq_new(const char *name, int oflag, long maxmsg, long msgsize)
{
	struct mq_attr at;
	mqd_t q;

	memset(&at, 0, sizeof at);
	at.mq_maxmsg = maxmsg;
	at.mq_msgsize = msgsize;
	q = mq_open(name, oflag | O_CREAT | O_EXCL, (mode_t)0600, maxmsg > 0 ? &at : NULL);
	NEED(q != BADQ, "mq_open(%s, create) failed: %s", name, pc_errname(errno));
	return q;
}

/* An existing queue opened again (setup). */
static inline mqd_t mq_again(const char *name, int oflag)
{
	mqd_t q = mq_open(name, oflag);

	NEED(q != BADQ, "mq_open(%s) failed: %s", name, pc_errname(errno));
	return q;
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* CLOCK_REALTIME now plus ms milliseconds (ms may be negative). */
static inline struct timespec abs_in(long ms)
{
	struct timespec t;

	NEED(clock_gettime(CLOCK_REALTIME, &t) == 0, "clock_gettime failed");
	t.tv_sec += ms / 1000;
	t.tv_nsec += (ms % 1000) * 1000000L;
	while (t.tv_nsec >= 1000000000L) {
		t.tv_nsec -= 1000000000L;
		t.tv_sec++;
	}
	while (t.tv_nsec < 0) {
		t.tv_nsec += 1000000000L;
		t.tv_sec--;
	}
	return t;
}

/* 1 if CLOCK_REALTIME has reached t. */
static inline int reached(const struct timespec *t)
{
	struct timespec n;

	NEED(clock_gettime(CLOCK_REALTIME, &n) == 0, "clock_gettime failed");
	return n.tv_sec > t->tv_sec || (n.tv_sec == t->tv_sec && n.tv_nsec >= t->tv_nsec);
}

/* Milliseconds from a (a CLOCK_REALTIME reading) to now. */
static inline long since_ms(const struct timespec *a)
{
	struct timespec n;

	NEED(clock_gettime(CLOCK_REALTIME, &n) == 0, "clock_gettime failed");
	return (long)(n.tv_sec - a->tv_sec) * 1000L + (n.tv_nsec - a->tv_nsec) / 1000000L;
}

/* mq_curmsgs of q (UNRESOLVED if mq_getattr fails). */
static inline long curmsgs(mqd_t q)
{
	struct mq_attr at;

	NEED(mq_getattr(q, &at) == 0, "mq_getattr failed: %s", pc_errname(errno));
	return at.mq_curmsgs;
}

/* Sends n one-byte messages of priority 0 (setup). */
static inline void fill(mqd_t q, int n)
{
	int i;

	for (i = 0; i < n; i++)
		NEED(mq_send(q, "x", 1, 0) == 0, "mq_send (fill %d) failed: %s", i, pc_errname(errno));
}

/* Waits for a child and returns its exit status (UNRESOLVED if it did not exit). */
static inline int reap(pid_t kid)
{
	int st;

	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st))
		pc_unresolved("a child did not exit (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static inline void nop_handler(int s)
{
	(void)s;
}

/*
 * A SIGALRM handler without SA_RESTART, so that an alarm() ends a wait with
 * EINTR instead of leaving a failing implementation blocked.
 */
static inline void alarm_handler_on(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = nop_handler;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
}

/* The highest priority value + 1 that mq_send accepts; UNRESOLVED if unknown. */
static inline unsigned prio_max(void)
{
#ifdef MQ_PRIO_MAX
	return (unsigned)MQ_PRIO_MAX;
#else
	long v = sysconf(_SC_MQ_PRIO_MAX);

	NEED(v > 0, "the number of priorities is not known");
	return (unsigned)v;
#endif
}

static inline void siginfo_nop(int s, siginfo_t *i, void *c)
{
	(void)s;
	(void)i;
	(void)c;
}

/*
 * Blocks SIGRTMIN in the calling thread (setup); *set holds just that signal.
 * SA_SIGINFO is set for it: only then is a signal generated with a value
 * (si_value) required to be queued with that value.
 */
static inline void rt_block(sigset_t *set)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = siginfo_nop;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGRTMIN, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(set);
	sigaddset(set, SIGRTMIN);
	NEED(pthread_sigmask(SIG_BLOCK, set, NULL) == 0, "pthread_sigmask failed");
}

/* Waits up to ms milliseconds for a signal of *set; 1 if one came (info filled). */
static inline int rt_wait(const sigset_t *set, siginfo_t *info, long ms)
{
	struct timespec ts;
	int r;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	do
		r = sigtimedwait(set, info, &ts);
	while (r == -1 && errno == EINTR);
	return r > 0;
}

/* A signal notification: SIGRTMIN with the value val. */
static inline struct sigevent ev_signal(int val)
{
	struct sigevent ev;

	memset(&ev, 0, sizeof ev);
	ev.sigev_notify = SIGEV_SIGNAL;
	ev.sigev_signo = SIGRTMIN;
	ev.sigev_value.sival_int = val;
	return ev;
}

#endif
