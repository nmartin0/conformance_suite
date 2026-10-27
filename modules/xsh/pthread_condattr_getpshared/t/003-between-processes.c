/*
 * A process-shared condition variable in shared memory works between a parent and a child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_getpshared.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED >= 0
struct shared {
	pthread_mutex_t m;
	pthread_cond_t c;
	int waiting, pred;	/* protected by m */
	int seen_pred, wait_result;	/* written by the child before done */
	atomic_int done;
};

static pid_t child_pid = -1;

static void reap_child(void)
{
	if (child_pid > 0) {
		(void)kill(child_pid, SIGKILL);
		while (waitpid(child_pid, NULL, 0) == -1 && errno == EINTR)
			;
		child_pid = -1;
	}
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	struct shared *s;
	pthread_mutexattr_t ma;
	pthread_condattr_t ca;
	int fd, r, i, st, w;

	pc_begin("XSH.pthread_condattr_getpshared.003");
	fd = open("shared", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)sizeof *s) == 0, "ftruncate failed: %s", pc_errname(errno));
	s = mmap(NULL, sizeof *s, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(s != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)close(fd);
	s->waiting = s->pred = s->seen_pred = 0;
	s->wait_result = -1;
	atomic_init(&s->done, 0);
	NEED(pthread_mutexattr_init(&ma) == 0, "pthread_mutexattr_init failed");
	NEED(pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_SHARED) == 0, "pthread_mutexattr_setpshared failed");
	NEED(pthread_mutex_init(&s->m, &ma) == 0, "pthread_mutex_init failed");
	NEED(pthread_condattr_init(&ca) == 0, "pthread_condattr_init failed");
	NEED(pthread_condattr_setpshared(&ca, PTHREAD_PROCESS_SHARED) == 0, "pthread_condattr_setpshared failed");
	NEED(pthread_cond_init(&s->c, &ca) == 0, "pthread_cond_init failed");
	(void)atexit(reap_child);
	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0) {
		int rc = 0;

		(void)pthread_mutex_lock(&s->m);
		s->waiting = 1;
		while (!s->pred && rc == 0)
			rc = pthread_cond_wait(&s->c, &s->m);
		s->wait_result = rc;
		s->seen_pred = s->pred;
		(void)pthread_mutex_unlock(&s->m);
		atomic_store(&s->done, 1);
		_exit(0);
	}
	pc_test();
	for (i = 0, w = 0; i < 2000 && !w; i++) {
		NEED(pthread_mutex_lock(&s->m) == 0, "pthread_mutex_lock in the parent failed");
		w = s->waiting;
		if (w) {
			/* the child has released the mutex inside pthread_cond_wait() */
			s->pred = 1;
			r = pthread_cond_signal(&s->c);
			pc_check(r == 0, "pthread_cond_signal() returned %s", rn(r));
		}
		(void)pthread_mutex_unlock(&s->m);
		if (!w)
			sleep_ms(5);
	}
	NEED(w, "the child did not start waiting");
	for (i = 0; i < 2000 && !atomic_load(&s->done); i++)
		sleep_ms(5);
	pc_check(atomic_load(&s->done), "the child blocked in pthread_cond_wait() was not woken within 10 s");
	pc_check(s->wait_result == 0, "in the child, pthread_cond_wait() returned %s", rn(s->wait_result));
	pc_check(s->seen_pred == 1, "the child did not see the predicate the parent set");
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_condattr_getpshared.003");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
