/*
 * A process-shared read-write lock in shared memory works between a parent and a child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlockattr_getpshared.003
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
	pthread_rwlock_t l;
	atomic_int step1, step2, go2, go3;
	atomic_int try1, try2;	/* the child's results */
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
	pthread_rwlockattr_t a;
	int fd, r, st;

	pc_begin("XSH.pthread_rwlockattr_getpshared.003");
	fd = open("shared", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)sizeof *s) == 0, "ftruncate failed: %s", pc_errname(errno));
	s = mmap(NULL, sizeof *s, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(s != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)close(fd);
	atomic_init(&s->step1, 0);
	atomic_init(&s->step2, 0);
	atomic_init(&s->go2, 0);
	atomic_init(&s->go3, 0);
	atomic_init(&s->try1, -1);
	atomic_init(&s->try2, -1);
	NEED(pthread_rwlockattr_init(&a) == 0, "pthread_rwlockattr_init failed");
	NEED(pthread_rwlockattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0, "pthread_rwlockattr_setpshared failed");
	NEED(pthread_rwlock_init(&s->l, &a) == 0, "pthread_rwlock_init failed");
	NEED(pthread_rwlock_wrlock(&s->l) == 0, "pthread_rwlock_wrlock failed");
	(void)atexit(reap_child);
	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0) {
		int t = pthread_rwlock_tryrdlock(&s->l);	/* parent holds a write lock */

		if (t == 0)
			(void)pthread_rwlock_unlock(&s->l);
		atomic_store(&s->try1, t);
		atomic_store(&s->step1, 1);
		while (!atomic_load(&s->go2))
			sleep_ms(2);
		t = pthread_rwlock_tryrdlock(&s->l);	/* parent has unlocked */
		atomic_store(&s->try2, t);
		atomic_store(&s->step2, 1);
		while (!atomic_load(&s->go3))
			sleep_ms(2);
		if (t == 0)
			(void)pthread_rwlock_unlock(&s->l);
		_exit(0);
	}
	pc_test();
	NEED(wait_flag(&s->step1, 10000), "the child did not run");
	r = atomic_load(&s->try1);
	pc_check(r == EBUSY, "in the child, pthread_rwlock_tryrdlock() with the parent holding a write lock returned %s, expected EBUSY", rn(r));
	r = pthread_rwlock_unlock(&s->l);
	pc_check(r == 0, "pthread_rwlock_unlock() in the parent returned %s", rn(r));
	atomic_store(&s->go2, 1);
	pc_check(wait_flag(&s->step2, 10000), "the child did not take its second step within 10 s");
	r = atomic_load(&s->try2);
	pc_check(r == 0, "in the child, pthread_rwlock_tryrdlock() after the parent unlocked returned %s", rn(r));
	r = pthread_rwlock_trywrlock(&s->l);
	pc_check(r == EBUSY, "in the parent, pthread_rwlock_trywrlock() with the child holding a read lock returned %s, expected EBUSY", rn(r));
	atomic_store(&s->go3, 1);
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	r = pthread_rwlock_trywrlock(&s->l);
	pc_check(r == 0, "in the parent, pthread_rwlock_trywrlock() after the child unlocked returned %s", rn(r));
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_rwlockattr_getpshared.003");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
