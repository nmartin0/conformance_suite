/*
 * A process-shared mutex in shared memory works between a parent and a child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getpshared.003
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
	atomic_int ready;	/* child: trylock result recorded */
	atomic_int tryres;
	atomic_int got;		/* child: lock returned */
	atomic_int lockres;
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

static int wait_flag(atomic_int *f, long ms)
{
	long w = 0;

	while (atomic_load(f) == 0 && w < ms) {
		sleep_ms(5);
		w += 5;
	}
	return atomic_load(f) != 0;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	struct shared *s;
	pthread_mutexattr_t a;
	int fd, r, st;

	pc_begin("XSH.pthread_mutexattr_getpshared.003");
	fd = open("shared", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)sizeof *s) == 0, "ftruncate failed: %s", pc_errname(errno));
	s = mmap(NULL, sizeof *s, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(s != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)close(fd);
	atomic_init(&s->ready, 0);
	atomic_init(&s->tryres, -1);
	atomic_init(&s->got, 0);
	atomic_init(&s->lockres, -1);
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	NEED(pthread_mutexattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0, "pthread_mutexattr_setpshared failed");
	NEED(pthread_mutex_init(&s->m, &a) == 0, "pthread_mutex_init failed");
	NEED(pthread_mutex_lock(&s->m) == 0, "pthread_mutex_lock failed");
	(void)atexit(reap_child);
	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0) {
		int t = pthread_mutex_trylock(&s->m);

		if (t == 0)
			(void)pthread_mutex_unlock(&s->m);
		atomic_store(&s->tryres, t);
		atomic_store(&s->ready, 1);
		t = pthread_mutex_lock(&s->m);
		atomic_store(&s->lockres, t);
		if (t == 0)
			(void)pthread_mutex_unlock(&s->m);
		atomic_store(&s->got, 1);
		_exit(0);
	}
	pc_test();
	NEED(wait_flag(&s->ready, 10000), "the child did not run");
	r = atomic_load(&s->tryres);
	pc_check(r == EBUSY, "in the child, pthread_mutex_trylock() of the mutex held by the parent returned %s, expected EBUSY", rn(r));
	sleep_ms(300);
	pc_check(atomic_load(&s->got) == 0, "in the child, pthread_mutex_lock() returned %s while the parent held the mutex",
	    rn(atomic_load(&s->lockres)));
	r = pthread_mutex_unlock(&s->m);
	pc_check(r == 0, "pthread_mutex_unlock() in the parent returned %s", rn(r));
	pc_check(wait_flag(&s->got, 10000), "in the child, pthread_mutex_lock() did not return within 10 s after the parent unlocked");
	r = atomic_load(&s->lockres);
	pc_check(r == 0, "in the child, pthread_mutex_lock() returned %s after the parent unlocked", rn(r));
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getpshared.003");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
