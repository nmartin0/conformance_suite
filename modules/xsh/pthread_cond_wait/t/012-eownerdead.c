/*
 * When the process owning a robust mutex dies while a thread waits on a
 * condition variable with it, the wait returns EOWNERDEAD with the mutex
 * acquired.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cond_wait.012
 * MUTANT    pthread_cond_wait.owner_dead_hidden
 * VARIANT   pthread_cond_wait.spurious
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

/* Setup steps after pc_test() that the assertion does not cover. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/*
 * Flags shared between threads, each read and written under flag_lock (a
 * mutex the test never waits on, so that no planted fault touches it).
 */
static pthread_mutex_t flag_lock = PTHREAD_MUTEX_INITIALIZER;
static int flags[16];

static void flag_set(int i, int v)
{
	(void)pthread_mutex_lock(&flag_lock);
	flags[i] = v;
	(void)pthread_mutex_unlock(&flag_lock);
}

static int flag_get(int i)
{
	int v;

	(void)pthread_mutex_lock(&flag_lock);
	v = flags[i];
	(void)pthread_mutex_unlock(&flag_lock);
	return v;
}

static void flag_add(int i, int d)
{
	(void)pthread_mutex_lock(&flag_lock);
	flags[i] += d;
	(void)pthread_mutex_unlock(&flag_lock);
}

static long now_ms(void)
{
	struct timespec ts;

	(void)clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for flag i to be at least v: 1 if it is. */
static int wait_flag(int i, int v, long ms)
{
	long end = now_ms() + ms;

	while (flag_get(i) < v) {
		if (now_ms() >= end)
			return 0;
		sleep_ms(1);
	}
	return 1;
}

/* The time on clock c, ms milliseconds from now (ms may be negative). */
static struct timespec ts_after(clockid_t c, long ms)
{
	struct timespec ts;

	(void)clock_gettime(c, &ts);
	ts.tv_sec += ms / 1000;
	ts.tv_nsec += (ms % 1000) * 1000000L;
	if (ts.tv_nsec >= 1000000000L) {
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	} else if (ts.tv_nsec < 0) {
		ts.tv_sec--;
		ts.tv_nsec += 1000000000L;
	}
	return ts;
}

/* 1 if a is not earlier than b. */
static int ts_ge(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec >= b.tv_nsec);
}

/*
 * Joins t through a joiner thread, waiting at most ms milliseconds, so
 * that a thread that never ends gives a verdict instead of a hang: 1 and
 * the thread's status in *ret if it was joined in time, 0 if not.
 */
static pthread_t join_target;
static void *join_status;

static void *joiner(void *arg)
{
	(void)arg;
	if (pthread_join(join_target, &join_status) == 0)
		flag_set(15, 1);
	else
		flag_set(15, 2);
	return NULL;
}

static int join_within(pthread_t t, long ms, void **ret)
{
	pthread_t j;

	flag_set(15, 0);
	join_target = t;
	if (pthread_create(&j, NULL, joiner, NULL) != 0)
		pc_unresolved("pthread_create (joiner) failed");
	if (!wait_flag(15, 1, ms))
		return 0;
	(void)pthread_join(j, NULL);
	if (flag_get(15) != 1)
		pc_unresolved("pthread_join failed");
	if (ret != NULL)
		*ret = join_status;
	return 1;
}

/* Initializes *m as a PTHREAD_MUTEX_ERRORCHECK mutex. */
static void errorcheck_mutex(pthread_mutex_t *m)
{
	pthread_mutexattr_t a;

	if (pthread_mutexattr_init(&a) != 0 || pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK) != 0 ||
	    pthread_mutex_init(m, &a) != 0)
		pc_unresolved("cannot make an error-checking mutex");
	(void)pthread_mutexattr_destroy(&a);
}

/* Starts fn(arg) in a new thread with default attributes. */
static pthread_t start(void *(*fn)(void *), void *arg)
{
	pthread_t t;

	if (pthread_create(&t, NULL, fn, arg) != 0)
		pc_unresolved("pthread_create failed");
	return t;
}

#include <sys/mman.h>
#include <sys/wait.h>

struct shared {
	pthread_mutex_t m;	/* robust, process-shared */
	int waiting;		/* under m */
};

static struct shared *sh;
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static int r_wait, r_consistent, r_unlock;

static void *waiter(void *arg)
{
	int r = 0;

	(void)arg;
	if (pthread_mutex_lock(&sh->m) != 0) {
		r_wait = -1;
		flag_set(1, 1);
		return NULL;
	}
	sh->waiting = 1;
	/* Only the main thread's signal (flag 2) or an error ends the loop. */
	while (r == 0 && !flag_get(2))
		r = pthread_cond_wait(&c, &sh->m);
	r_wait = r;
	if (r == EOWNERDEAD)
		r_consistent = pthread_mutex_consistent(&sh->m);
	r_unlock = pthread_mutex_unlock(&sh->m);
	flag_set(1, 1);
	return NULL;
}

int main(void)
{
	pthread_mutexattr_t a;
	pthread_t t;
	pid_t kid;
	long end;
	int st, k;

	pc_begin("XSH.pthread_cond_wait.012");
	sh = mmap(NULL, sizeof *sh, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	NEED(sh != MAP_FAILED, "mmap failed");
	NEED(pthread_mutexattr_init(&a) == 0 && pthread_mutexattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0 &&
	     pthread_mutexattr_setrobust(&a, PTHREAD_MUTEX_ROBUST) == 0 && pthread_mutex_init(&sh->m, &a) == 0,
	     "cannot make a robust process-shared mutex");
	/* The child is made while this process has one thread. */
	kid = fork();
	NEED(kid != -1, "fork failed");
	if (kid == 0) {
		/* Take the mutex once the waiter has released it in the wait; die holding it. */
		end = now_ms() + 20000;
		while (now_ms() < end) {
			if (pthread_mutex_lock(&sh->m) == 0) {
				if (sh->waiting)
					_exit(0);
				(void)pthread_mutex_unlock(&sh->m);
			}
			sleep_ms(1);
		}
		_exit(1);
	}
	pc_test();
	t = start(waiter, NULL);
	while (waitpid(kid, &st, 0) == -1)
		NEED(errno == EINTR, "waitpid failed");
	NEED(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child did not take the mutex while the thread waited");
	flag_set(2, 1);
	/*
	 * Signal until the wait returns: only a thread that took the mutex
	 * after the waiter released it is promised that its signal comes
	 * after the waiter blocked, and this thread never took it.
	 */
	for (k = 0; k < 10000 && flag_get(1) == 0; k++) {
		pc_check(pthread_cond_signal(&c) == 0, "pthread_cond_signal failed");
		sleep_ms(1);
	}
	pc_check(wait_flag(1, 1, 1000), "the wait did not return within 10 s after the mutex's owner died and it was signaled");
	(void)pthread_join(t, NULL);
	NEED(r_wait != -1, "the waiter could not lock the mutex");
	pc_check(r_wait == EOWNERDEAD, "the wait returned %s, not EOWNERDEAD, after the process owning the robust mutex died",
		 r_wait == 0 ? "0" : pc_errname(r_wait));
	pc_check(r_consistent == 0, "after EOWNERDEAD pthread_mutex_consistent() failed (%s): the mutex was not acquired",
		 pc_errname(r_consistent));
	pc_check(r_unlock == 0, "after EOWNERDEAD the caller could not unlock the mutex (%s)", pc_errname(r_unlock));
	pc_pass();
}
