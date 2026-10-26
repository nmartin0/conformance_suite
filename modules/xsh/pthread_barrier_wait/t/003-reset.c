/*
 * A barrier is reset after each completion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_barrier_wait.003
 * MUTANT    pthread_barrier_wait.no_reset
 * MUTANT    pthread_barrier_wait.serial_once
 * VARIANT   pthread_barrier_wait.serial_first
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero; 1 if it did. */
static int wait_flag(atomic_int *flag, long ms)
{
	long waited = 0;

	while (atomic_load(flag) == 0 && waited < ms) {
		sleep_ms(5);
		waited += 5;
	}
	return atomic_load(flag) != 0;
}

/* Waits up to ms milliseconds for *v to exceed old; 1 if it did. */
static int wait_flag_above(atomic_int *v, int old, long ms)
{
	long waited = 0;

	while (atomic_load(v) <= old && waited < ms) {
		sleep_ms(5);
		waited += 5;
	}
	return atomic_load(v) > old;
}

/*
 * A job: fn(arg) run in a thread of its own. Threads report only through
 * these atomic fields, never through the locks under test.
 */
struct job {
	int (*fn)(void *);
	void *arg;
	atomic_int started, done;
	int result;
	pthread_t thread;
};

static void *job_main(void *p)
{
	struct job *j = p;

	atomic_store(&j->started, 1);
	j->result = j->fn(j->arg);
	atomic_store(&j->done, 1);
	return NULL;
}

/* Starts fn(arg) in a new thread and waits until it has started. */
static void job_start(struct job *j, int (*fn)(void *), void *arg)
{
	j->fn = fn;
	j->arg = arg;
	atomic_store(&j->started, 0);
	atomic_store(&j->done, 0);
	j->result = -1;
	NEED(pthread_create(&j->thread, NULL, job_main, j) == 0, "pthread_create failed");
	NEED(wait_flag(&j->started, 10000), "the new thread did not start");
}

/* Waits for a job that must finish (UNRESOLVED if it does not in 10 s). */
static int job_finish(struct job *j, const char *what)
{
	if (!wait_flag(&j->done, 10000))
		pc_unresolved("%s did not return within 10 s", what);
	NEED(pthread_join(j->thread, NULL) == 0, "pthread_join failed");
	return j->result;
}

/*
 * Runs fn(arg), a call that never waits, in another thread and returns
 * its result; one that has not returned in 10 s is a failed check.
 */
static int in_other_thread(int (*fn)(void *), void *arg, const char *what)
{
	struct job j;

	job_start(&j, fn, arg);
	pc_check(wait_flag(&j.done, 10000), "%s in another thread did not return within 10 s", what);
	NEED(pthread_join(j.thread, NULL) == 0, "pthread_join failed");
	return j.result;
}

/* "0" or the error's name, for messages. */
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static atomic_int handled;

static void on_signal(int sig)
{
	(void)sig;
	atomic_fetch_add(&handled, 1);
}

/* Catches SIGUSR1 with on_signal(), without SA_RESTART (setup). */
static void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction(SIGUSR1) failed: %s", pc_errname(errno));
}

/* Sends SIGUSR1 to a job's thread and waits for the handler to have run. */
static void signal_job(struct job *j)
{
	int before = atomic_load(&handled);

	NEED(pthread_kill(j->thread, SIGUSR1) == 0, "pthread_kill failed");
	NEED(wait_flag_above(&handled, before, 10000), "the SIGUSR1 handler did not run within 10 s");
}

static int do_wait(void *b) { return pthread_barrier_wait(b); }

static void new_barrier(pthread_barrier_t *b, unsigned count)
{
	NEED(pthread_barrier_init(b, NULL, count) == 0, "pthread_barrier_init(%u) failed", count);
}

/* 0 or PTHREAD_BARRIER_SERIAL_THREAD: a successful return. */
static int ok_return(int r)
{
	return r == 0 || r == PTHREAD_BARRIER_SERIAL_THREAD;
}

static const char *bn(int r)
{
	return r == PTHREAD_BARRIER_SERIAL_THREAD ? "PTHREAD_BARRIER_SERIAL_THREAD" : rn(r);
}

#define ROUNDS 4

struct worker {
	pthread_barrier_t *b;
	atomic_int entered, returned;
	int res[ROUNDS];
};

static int worker_main(void *p)
{
	struct worker *w = p;
	int k;

	for (k = 0; k < ROUNDS; k++) {
		atomic_store(&w->entered, k + 1);
		w->res[k] = pthread_barrier_wait(w->b);
		atomic_store(&w->returned, k + 1);
	}
	return 0;
}

/* Waits up to 10 s for *v to reach want; 1 if it did. */
static int wait_for(atomic_int *v, int want)
{
	long waited = 0;

	while (atomic_load(v) < want && waited < 10000) {
		sleep_ms(5);
		waited += 5;
	}
	return atomic_load(v) >= want;
}

int main(void)
{
	static pthread_barrier_t b;
	static struct worker w[2];
	struct job j[2];
	int k, i, r, serial;

	pc_begin("XSH.pthread_barrier_wait.003");
	new_barrier(&b, 3);
	pc_test();
	for (i = 0; i < 2; i++) {
		w[i].b = &b;
		atomic_store(&w[i].entered, 0);
		atomic_store(&w[i].returned, 0);
		job_start(&j[i], worker_main, &w[i]);
	}
	for (k = 0; k < ROUNDS; k++) {
		for (i = 0; i < 2; i++)
			NEED(wait_for(&w[i].entered, k + 1), "a worker did not reach round %d", k + 1);
		sleep_ms(300);
		for (i = 0; i < 2; i++)
			pc_check(atomic_load(&w[i].returned) == k,
			    "in round %d, a pthread_barrier_wait() returned (%s) with 2 of 3 threads at the barrier", k + 1,
			    bn(w[i].res[k]));
		r = pthread_barrier_wait(&b);
		pc_check(ok_return(r), "in round %d, the third pthread_barrier_wait() returned %s", k + 1, bn(r));
		serial = r == PTHREAD_BARRIER_SERIAL_THREAD;
		for (i = 0; i < 2; i++) {
			if (!wait_for(&w[i].returned, k + 1))
				pc_unresolved("in round %d, a waiter had not returned 10 s after the third thread arrived", k + 1);
			pc_check(ok_return(w[i].res[k]), "in round %d, a waiter's pthread_barrier_wait() returned %s", k + 1,
			    bn(w[i].res[k]));
			serial += w[i].res[k] == PTHREAD_BARRIER_SERIAL_THREAD;
		}
		pc_check(serial == 1, "in round %d, %d threads got PTHREAD_BARRIER_SERIAL_THREAD, expected exactly 1", k + 1,
		    serial);
	}
	for (i = 0; i < 2; i++)
		(void)job_finish(&j[i], "a worker");
	pc_pass();
}
