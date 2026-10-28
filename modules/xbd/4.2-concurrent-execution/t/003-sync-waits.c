/*
 * A thread blocked in pthread_mutex_lock(), pthread_cond_wait(), sem_wait(),
 * pthread_join(), pthread_rwlock_wrlock() or pthread_barrier_wait() does not
 * keep the thread that would unblock it from running.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.2-concurrent-execution.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define NCASES 6

static const char *names[NCASES] = { "pthread_mutex_lock", "pthread_cond_wait", "sem_wait", "pthread_join", "pthread_rwlock_wrlock", "pthread_barrier_wait" };
static int which;
static volatile int blocked_done;
static pthread_mutex_t mu;
static pthread_mutex_t cmu = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static int condflag;
static sem_t sem;
static pthread_rwlock_t rw;
static pthread_barrier_t bar;
static volatile int joinee_go;

static void on_alarm(int s)
{
	(void)s;
	pc_fail("timed out: a thread blocked in %s kept the releasing thread from running", names[which]);
}

static void *joinee(void *arg)
{
	struct timespec nap = { 0, 5000000L };

	(void)arg;
	while (!joinee_go)
		(void)nanosleep(&nap, NULL);
	return NULL;
}

static pthread_t tj;

static void *blocker(void *arg)
{
	(void)arg;
	switch (which) {
	case 0:
		if (pthread_mutex_lock(&mu) == 0) {
			(void)pthread_mutex_unlock(&mu);
			blocked_done = 1;
		}
		break;
	case 1:
		(void)pthread_mutex_lock(&cmu);
		while (!condflag)
			(void)pthread_cond_wait(&cv, &cmu);
		(void)pthread_mutex_unlock(&cmu);
		blocked_done = 1;
		break;
	case 2:
		while (sem_wait(&sem) == -1 && errno == EINTR)
			;
		blocked_done = 1;
		break;
	case 3:
		if (pthread_join(tj, NULL) == 0)
			blocked_done = 1;
		break;
	case 4:
		if (pthread_rwlock_wrlock(&rw) == 0) {
			(void)pthread_rwlock_unlock(&rw);
			blocked_done = 1;
		}
		break;
	default:
		(void)pthread_barrier_wait(&bar);
		blocked_done = 1;
		break;
	}
	return NULL;
}

static void *releaser(void *arg)
{
	struct timespec nap = { 0, 100000000L };

	(void)arg;
	(void)nanosleep(&nap, NULL);	/* let the other thread block first */
	switch (which) {
	case 1:
		(void)pthread_mutex_lock(&cmu);
		condflag = 1;
		(void)pthread_cond_broadcast(&cv);
		(void)pthread_mutex_unlock(&cmu);
		break;
	case 2:
		(void)sem_post(&sem);
		break;
	case 3:
		joinee_go = 1;
		break;
	default:
		(void)pthread_barrier_wait(&bar);
		break;
	}
	return NULL;
}

int main(void)
{
	pthread_t tb, tr;
	int k, r;

	pc_begin("XBD.4.2-concurrent-execution.003");
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(60);
	for (k = 0; k < NCASES; k++) {
		which = k;
		blocked_done = 0;
		condflag = 0;
		joinee_go = 0;
		if (pthread_mutex_init(&mu, NULL) != 0 || sem_init(&sem, 0, 0) != 0 ||
		    pthread_rwlock_init(&rw, NULL) != 0 || pthread_barrier_init(&bar, NULL, 2) != 0)
			pc_unresolved("cannot initialise the synchronisation objects: %s", pc_errname(errno));
		/* the lock is held here, by the main thread, for the blocker to wait on */
		if (k == 0 && pthread_mutex_lock(&mu) != 0)
			pc_unresolved("pthread_mutex_lock failed");
		if (k == 4 && pthread_rwlock_wrlock(&rw) != 0)
			pc_unresolved("pthread_rwlock_wrlock failed");
		if (k == 3 && (r = pthread_create(&tj, NULL, joinee, NULL)) != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		r = pthread_create(&tb, NULL, blocker, NULL);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		/* a lock is released by a thread that does not own it only where the type allows it,
		 * so the releasing thread takes the main thread's place: main hands the lock over */
		if (k == 0 || k == 4) {
			struct timespec nap = { 0, 100000000L };

			(void)nanosleep(&nap, NULL);
			if (k == 0)
				(void)pthread_mutex_unlock(&mu);
			else
				(void)pthread_rwlock_unlock(&rw);
			(void)pthread_join(tb, NULL);
		} else {
			r = pthread_create(&tr, NULL, releaser, NULL);
			if (r != 0)
				pc_unresolved("pthread_create: %s", pc_errname(r));
			(void)pthread_join(tr, NULL);
			(void)pthread_join(tb, NULL);
		}
		pc_check(blocked_done, "the thread blocked in %s did not complete", names[k]);
		(void)pthread_barrier_destroy(&bar);
		(void)pthread_rwlock_destroy(&rw);
		(void)sem_destroy(&sem);
		(void)pthread_mutex_destroy(&mu);
	}
	pc_pass();
}
