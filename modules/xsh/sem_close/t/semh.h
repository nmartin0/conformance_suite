/*
 * semh.h - helpers shared by the semaphore tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Every function is static inline so that a test uses only what it needs.
 * Helper threads report through atomic flags, never through the
 * semaphores under test.
 */
#ifndef SEMH_H
#define SEMH_H

#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* A step that is not under test: failing it is UNRESOLVED. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* "0" or the error's name, for messages. */
static inline const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero; 1 if it did. */
static inline int wait_flag(atomic_int *flag, long ms)
{
	long waited = 0;

	while (atomic_load(flag) == 0 && waited < ms) {
		sleep_ms(5);
		waited += 5;
	}
	return atomic_load(flag) != 0;
}

/* A call that blocks for good must not hang the suite. */
static inline void on_guard(int sig)
{
	(void)sig;
	pc_check(0, "a call did not return within the guard time");
}

static inline void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction(SIGALRM) failed");
	alarm(secs);
}

/*
 * A job: fn(arg) run in a thread of its own. result and err (errno after
 * the call) are valid once done is set.
 */
struct job {
	int (*fn)(void *);
	void *arg;
	atomic_int started, done;
	int result, err;
	pthread_t thread;
};

static inline void *job_main(void *p)
{
	struct job *j = p;
	int r;

	atomic_store(&j->started, 1);
	errno = 0;
	r = j->fn(j->arg);
	j->err = errno;
	j->result = r;
	atomic_store(&j->done, 1);
	return NULL;
}

/* Starts fn(arg) in a new thread and waits until the thread has started. */
static inline void job_start(struct job *j, int (*fn)(void *), void *arg)
{
	j->fn = fn;
	j->arg = arg;
	atomic_store(&j->started, 0);
	atomic_store(&j->done, 0);
	j->result = -2;
	j->err = 0;
	NEED(pthread_create(&j->thread, NULL, job_main, j) == 0, "pthread_create failed");
	NEED(wait_flag(&j->started, 10000), "the new thread did not start");
}

/* Waits (10 s at most) for a job that must finish, then joins it. */
static inline void job_finish(struct job *j, const char *what)
{
	pc_check(wait_flag(&j->done, 10000), "%s did not return within 10 s", what);
	NEED(pthread_join(j->thread, NULL) == 0, "pthread_join failed");
}

static inline int do_wait(void *s) { return sem_wait(s); }
static inline int do_trywait(void *s) { return sem_trywait(s); }

/* The largest valid semaphore value, or -1 if it is not determinate. */
static inline long semmax(void)
{
#ifdef SEM_VALUE_MAX
	return (long)SEM_VALUE_MAX;
#else
	return sysconf(_SC_SEM_VALUE_MAX);
#endif
}

/*
 * Counts successful sem_trywait() calls until the first failure, which
 * must be EAGAIN (anything else is a failed check).
 */
static inline int drain(sem_t *s)
{
	int n = 0;

	for (;;) {
		if (sem_trywait(s) == 0) {
			pc_check(++n < 100000, "sem_trywait() kept succeeding");
			continue;
		}
		pc_check(errno == EAGAIN, "sem_trywait() failed with %s, expected success or EAGAIN",
		    pc_errname(errno));
		return n;
	}
}

static inline void expect_count(sem_t *s, int n, const char *what)
{
	int got = drain(s);

	pc_check(got == n, "%s: %d successful sem_trywait() calls, expected %d", what, got, n);
}

/* The semaphore's value by sem_getvalue(), where a failure is UNRESOLVED. */
static inline int value_of(sem_t *s)
{
	int v = 0;

	NEED(sem_getvalue(s, &v) == 0, "sem_getvalue failed: %s", pc_errname(errno));
	return v;
}

static inline void new_sem(sem_t *s, int pshared, unsigned value)
{
	NEED(sem_init(s, pshared, value) == 0, "sem_init(%u) failed: %s", value, pc_errname(errno));
}

/* Clocks and timeouts. */
static inline void clk_now(clockid_t c, struct timespec *t)
{
	NEED(clock_gettime(c, t) == 0, "clock_gettime failed: %s", pc_errname(errno));
}

static inline void ts_add_ms(struct timespec *t, long ms)
{
	t->tv_sec += ms / 1000;
	t->tv_nsec += (ms % 1000) * 1000000L;
	if (t->tv_nsec >= 1000000000L) {
		t->tv_sec++;
		t->tv_nsec -= 1000000000L;
	}
}

/* 1 if a is later than or equal to b. */
static inline int ts_ge(const struct timespec *a, const struct timespec *b)
{
	return a->tv_sec > b->tv_sec || (a->tv_sec == b->tv_sec && a->tv_nsec >= b->tv_nsec);
}

/* The three ways to wait with a timeout. */
struct wf {
	const char *name;
	int kind;		/* 0: sem_timedwait, 1: sem_clockwait */
	clockid_t clk;
};

static const struct wf wfs[3] = {
	{ "sem_timedwait()", 0, CLOCK_REALTIME },
	{ "sem_clockwait(CLOCK_REALTIME)", 1, CLOCK_REALTIME },
	{ "sem_clockwait(CLOCK_MONOTONIC)", 1, CLOCK_MONOTONIC },
};

static inline int do_timed(const struct wf *w, sem_t *s, const struct timespec *t)
{
	return w->kind == 0 ? sem_timedwait(s, t) : sem_clockwait(s, w->clk, t);
}

/* A timeout in ms from now, on the clock of w. */
static inline struct timespec deadline(const struct wf *w, long ms)
{
	struct timespec t;

	clk_now(w->clk, &t);
	ts_add_ms(&t, ms);
	return t;
}

/* A child's exit status; a child that did not exit normally is UNRESOLVED. */
static inline int reap(pid_t pid)
{
	int st;

	while (waitpid(pid, &st, 0) == -1)
		NEED(errno == EINTR, "waitpid failed");
	NEED(WIFEXITED(st), "a child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

/* SIGUSR1 handling without SA_RESTART. */
static atomic_int handled;

static inline void on_usr1(int sig)
{
	(void)sig;
	atomic_fetch_add(&handled, 1);
}

static inline void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_usr1;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction(SIGUSR1) failed");
}

/* Sends SIGUSR1 to a job's thread and waits (10 s) for the handler to run. */
static inline void signal_job(struct job *j)
{
	int before = atomic_load(&handled);
	long waited = 0;

	NEED(pthread_kill(j->thread, SIGUSR1) == 0, "pthread_kill failed");
	while (atomic_load(&handled) == before && waited < 10000) {
		sleep_ms(5);
		waited += 5;
	}
	NEED(atomic_load(&handled) > before, "the SIGUSR1 handler did not run within 10 s");
}

/*
 * Names of named semaphores: "/pcsem<pid><tag>", unique to the test
 * process, removed at exit (in the process that made them only).
 */
static char sn_names[8][64];
static int sn_n;
static pid_t sn_owner;

static inline void sn_cleanup(void)
{
	int i;

	if (getpid() != sn_owner)
		return;
	for (i = 0; i < sn_n; i++)
		(void)sem_unlink(sn_names[i]);
}

static inline const char *sn_make(const char *tag)
{
	if (sn_n == 0) {
		sn_owner = getpid();
		NEED(atexit(sn_cleanup) == 0, "atexit failed");
	}
	NEED(sn_n < 8, "too many semaphore names");
	snprintf(sn_names[sn_n], sizeof sn_names[sn_n], "/pcsem%ld%s", (long)getpid(), tag);
	(void)sem_unlink(sn_names[sn_n]);
	return sn_names[sn_n++];
}

/* Opens a new named semaphore (exclusively) with the given value; failing is UNRESOLVED. */
static inline sem_t *sn_create(const char *name, unsigned value)
{
	sem_t *s = sem_open(name, O_CREAT | O_EXCL, (mode_t)0600, value);

	NEED(s != SEM_FAILED, "sem_open(O_CREAT|O_EXCL) failed: %s", pc_errname(errno));
	return s;
}

#endif
