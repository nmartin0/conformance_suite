/*
 * The new thread inherits the creator's signal mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.thrd_create.004
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef __STDC_NO_THREADS__
int main(void)
{
	pc_begin("XSH.thrd_create.004");
	pc_notinuse("<threads.h> is not provided");
}
#else
#include <threads.h>
#define TEST_ID "XSH.thrd_create.004"

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static inline long now_ms(void)
{
	struct timespec ts;

	(void)clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

/* Waits up to ms milliseconds for *v to be at least want; 1 if it is. */
static inline int wait_ge(atomic_int *v, int want, long ms)
{
	long end = now_ms() + ms;

	while (atomic_load(v) < want) {
		if (now_ms() >= end)
			return 0;
		sleep_ms(2);
	}
	return 1;
}

static inline int wait_flag(atomic_int *v, long ms)
{
	return wait_ge(v, 1, ms);
}

/* The TIME_UTC calendar time ms milliseconds from now (ms may be negative). */
static inline struct timespec utc_after(long ms)
{
	struct timespec ts;

	if (timespec_get(&ts, TIME_UTC) != TIME_UTC)
		pc_unresolved("timespec_get(TIME_UTC) failed");
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
static inline int ts_ge(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec >= b.tv_nsec);
}

/* a - b in nanoseconds. */
static inline long long ts_diff_ns(struct timespec a, struct timespec b)
{
	return (long long)(a.tv_sec - b.tv_sec) * 1000000000LL + (a.tv_nsec - b.tv_nsec);
}

static inline const char *rn(int r)
{
	switch (r) {
	case thrd_success: return "thrd_success";
	case thrd_busy: return "thrd_busy";
	case thrd_error: return "thrd_error";
	case thrd_nomem: return "thrd_nomem";
	case thrd_timedout: return "thrd_timedout";
	}
	return "an unknown value";
}

/*
 * A job: fn(arg) run in a thread of its own. Jobs report only through
 * these atomic fields.
 */
struct job {
	int (*fn)(void *);
	void *arg;
	atomic_int started, done;
	int result;
	thrd_t thr;
};

static inline int job_main(void *p)
{
	struct job *j = p;

	atomic_store(&j->started, 1);
	j->result = j->fn(j->arg);
	atomic_store(&j->done, 1);
	return j->result;
}

static inline void job_start(struct job *j, int (*fn)(void *), void *arg)
{
	j->fn = fn;
	j->arg = arg;
	atomic_store(&j->started, 0);
	atomic_store(&j->done, 0);
	j->result = -1;
	NEED(thrd_create(&j->thr, job_main, j) == thrd_success, "thrd_create failed");
	NEED(wait_flag(&j->started, 10000), "the new thread did not start");
}

/* Waits for a job that must finish (UNRESOLVED if it does not in 10 s) and joins it. */
static inline int job_finish(struct job *j, const char *what)
{
	if (!wait_flag(&j->done, 10000))
		pc_unresolved("%s did not return within 10 s", what);
	NEED(thrd_join(j->thr, NULL) == thrd_success, "thrd_join failed");
	return j->result;
}

/* Runs fn(arg), a call that never waits, in another thread; one that has not returned in 10 s is a FAIL. */
static inline int in_other_thread(int (*fn)(void *), void *arg, const char *what)
{
	struct job j;

	job_start(&j, fn, arg);
	pc_check(wait_flag(&j.done, 10000), "%s in another thread did not return within 10 s", what);
	NEED(thrd_join(j.thr, NULL) == thrd_success, "thrd_join failed");
	return j.result;
}

static inline int do_lock_unlock(void *m)
{
	int r = mtx_lock(m);

	if (r == thrd_success)
		(void)mtx_unlock(m);
	return r;
}

static inline int do_trylock_unlock(void *m)
{
	int r = mtx_trylock(m);

	if (r == thrd_success)
		(void)mtx_unlock(m);
	return r;
}

/* Initializes m (UNRESOLVED if that fails). */
static inline void new_mtx(mtx_t *m, int type)
{
	NEED(mtx_init(m, type) == thrd_success, "mtx_init failed");
}

/* Another thread's mtx_lock() on m: its result (the job must return within 10 s, else FAIL). */
static inline int other_lock(mtx_t *m)
{
	return in_other_thread(do_lock_unlock, m, "mtx_lock()");
}

static inline int other_trylock(mtx_t *m)
{
	return in_other_thread(do_trylock_unlock, m, "mtx_trylock()");
}

/* A holder: another thread that locks m, tells so, and unlocks after hold_ms (if > 0) or when release is set. */
struct holder {
	mtx_t *m;
	long hold_ms;
	atomic_int locked, release;
	struct job job;
};

static inline int hold_main(void *p)
{
	struct holder *h = p;
	int r = mtx_lock(h->m);

	if (r != thrd_success) {
		atomic_store(&h->locked, 2);
		return r;
	}
	atomic_store(&h->locked, 1);
	if (h->hold_ms > 0)
		sleep_ms(h->hold_ms);
	else
		while (atomic_load(&h->release) == 0)
			sleep_ms(2);
	return mtx_unlock(h->m);
}

static inline void hold_start(struct holder *h, mtx_t *m, long hold_ms)
{
	h->m = m;
	h->hold_ms = hold_ms;
	atomic_store(&h->locked, 0);
	atomic_store(&h->release, 0);
	job_start(&h->job, hold_main, h);
	NEED(wait_flag(&h->locked, 10000) && atomic_load(&h->locked) == 1, "the holder thread could not lock the mutex");
}

static inline int hold_end(struct holder *h)
{
	atomic_store(&h->release, 1);
	return job_finish(&h->job, "the holder thread");
}

/* Signal helpers: a thread announces itself, another sends it SIGUSR1. */
static atomic_int handled, target_set;
static pthread_t target;

static inline void use_vars(void)
{
	(void)handled;
	(void)target_set;
	(void)target;
}

static inline void on_signal(int sig)
{
	(void)sig;
	atomic_fetch_add(&handled, 1);
}

static inline void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction(SIGUSR1) failed: %s", pc_errname(errno));
}

/* Called by the thread that is to be signalled. */
static inline void announce(void)
{
	target = pthread_self();
	atomic_store(&target_set, 1);
}

static inline void signal_target(void)
{
	int before = atomic_load(&handled);

	NEED(wait_flag(&target_set, 10000), "the target thread did not announce itself");
	NEED(pthread_kill(target, SIGUSR1) == 0, "pthread_kill failed");
	NEED(wait_ge(&handled, before + 1, 10000), "the SIGUSR1 handler did not run within 10 s");
}

/*
 * Waiters on a condition variable: each locks m, counts itself in
 * waiting, waits once (no predicate) and counts itself in woken.
 */
struct wt {
	mtx_t *m;
	cnd_t *c;
	atomic_int waiting, woken, back, rc;
	long ts_ms;	/* != 0: cnd_timedwait() with that offset from now */
	long hold_ms;	/* time to keep the mutex after waking */
};

static inline int waiter_main(void *p)
{
	struct wt *w = p;
	struct timespec ts;
	int r;

	NEED(mtx_lock(w->m) == thrd_success, "mtx_lock failed");
	atomic_fetch_add(&w->waiting, 1);
	if (w->ts_ms != 0) {
		ts = utc_after(w->ts_ms);
		r = cnd_timedwait(w->c, w->m, &ts);
	} else {
		r = cnd_wait(w->c, w->m);
	}
	atomic_store(&w->rc, r);
	atomic_store(&w->back, 1);
	atomic_fetch_add(&w->woken, 1);
	if (w->hold_ms > 0)
		sleep_ms(w->hold_ms);
	(void)mtx_unlock(w->m);
	return r;
}

/* Waits until n waiters are inside their wait, then locks w->m (the caller unlocks it). */
static inline void all_blocked(struct wt *w, int n)
{
	NEED(wait_ge(&w->waiting, n, 10000), "the waiters did not start");
	NEED(mtx_lock(w->m) == thrd_success, "mtx_lock failed");
}

static int fn(void *p)
{
	sigset_t s;

	(void)p;
	if (pthread_sigmask(SIG_BLOCK, NULL, &s) != 0)
		return -1;
	return (sigismember(&s, SIGUSR1) == 1 ? 1 : 0) | (sigismember(&s, SIGUSR2) == 1 ? 2 : 0);
}

int main(void)
{
	sigset_t s;
	thrd_t t;
	int r, res = -9;

	pc_begin(TEST_ID);
	sigemptyset(&s);
	sigaddset(&s, SIGUSR2);
	NEED(pthread_sigmask(SIG_UNBLOCK, &s, NULL) == 0, "pthread_sigmask failed");
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	NEED(pthread_sigmask(SIG_BLOCK, &s, NULL) == 0, "pthread_sigmask failed");
	pc_test();
	r = thrd_create(&t, fn, NULL);
	pc_check(r == thrd_success, "thrd_create() returned %s", rn(r));
	NEED(thrd_join(t, &res) == thrd_success, "thrd_join failed");
	pc_check(res >= 0, "the new thread could not read its signal mask");
	pc_check((res & 1) == 1, "SIGUSR1, blocked in the creator, is not blocked in the new thread");
	pc_check((res & 2) == 0, "SIGUSR2, unblocked in the creator, is blocked in the new thread");
	pc_pass();
}
#endif
