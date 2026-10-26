/*
 * Exactly one thread gets PTHREAD_BARRIER_SERIAL_THREAD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_barrier_wait.002
 * MUTANT    pthread_barrier_wait.serial_none
 * MUTANT    pthread_barrier_wait.serial_all
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

int main(void)
{
	static pthread_barrier_t b, one;
	struct job a1, a2;
	int res[3], serial = 0, i, r;

	pc_begin("XSH.pthread_barrier_wait.002");
	new_barrier(&b, 3);
	new_barrier(&one, 1);
	pc_test();
	pc_check(PTHREAD_BARRIER_SERIAL_THREAD != 0, "PTHREAD_BARRIER_SERIAL_THREAD is 0");
	job_start(&a1, do_wait, &b);
	job_start(&a2, do_wait, &b);
	sleep_ms(100);
	res[0] = pthread_barrier_wait(&b);
	res[1] = job_finish(&a1, "the first waiter");
	res[2] = job_finish(&a2, "the second waiter");
	for (i = 0; i < 3; i++) {
		pc_check(ok_return(res[i]), "a pthread_barrier_wait() returned %s", bn(res[i]));
		serial += res[i] == PTHREAD_BARRIER_SERIAL_THREAD;
	}
	pc_check(serial == 1, "%d of the 3 threads got PTHREAD_BARRIER_SERIAL_THREAD, expected exactly 1", serial);
	r = pthread_barrier_wait(&one);
	pc_check(r == PTHREAD_BARRIER_SERIAL_THREAD,
	    "pthread_barrier_wait() on a barrier for 1 thread returned %s, expected PTHREAD_BARRIER_SERIAL_THREAD", bn(r));
	pc_pass();
}
