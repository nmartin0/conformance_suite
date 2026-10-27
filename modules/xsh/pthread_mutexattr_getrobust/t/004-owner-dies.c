/*
 * A robust mutex whose owner process has terminated reports EOWNERDEAD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getrobust.004
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

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

/* Locks m with a 10 s limit (a mutex that stalls gives ETIMEDOUT). */
static int lock10(pthread_mutex_t *m)
{
	struct timespec ts;

	NEED(clock_gettime(CLOCK_REALTIME, &ts) == 0, "clock_gettime failed");
	ts.tv_sec += 10;
	return pthread_mutex_timedlock(m, &ts);
}

int main(void)
{
	pthread_mutex_t *m;
	pthread_mutexattr_t a;
	int fd, r, st;

	pc_begin("XSH.pthread_mutexattr_getrobust.004");
	fd = open("shared", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)sizeof *m) == 0, "ftruncate failed: %s", pc_errname(errno));
	m = mmap(NULL, sizeof *m, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(m != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)close(fd);
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	NEED(pthread_mutexattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0, "pthread_mutexattr_setpshared failed");
	NEED(pthread_mutexattr_setrobust(&a, PTHREAD_MUTEX_ROBUST) == 0, "pthread_mutexattr_setrobust failed");
	NEED(pthread_mutex_init(m, &a) == 0, "pthread_mutex_init of a robust process-shared mutex failed");
	(void)atexit(reap_child);
	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0)
		_exit(pthread_mutex_lock(m) == 0 ? 0 : 3);	/* terminates holding the mutex */
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	NEED(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child could not lock the mutex (status %#x)", (unsigned)st);
	pc_test();
	r = lock10(m);
	pc_check(r == EOWNERDEAD, "pthread_mutex_timedlock() of a robust mutex whose owner process terminated returned %s, expected EOWNERDEAD", rn(r));
	r = pthread_mutex_consistent(m);
	pc_check(r == 0, "pthread_mutex_consistent() returned %s", rn(r));
	r = pthread_mutex_unlock(m);
	pc_check(r == 0, "pthread_mutex_unlock() returned %s", rn(r));
	r = lock10(m);
	pc_check(r == 0, "pthread_mutex_timedlock() after consistent and unlock returned %s", rn(r));
	r = pthread_mutex_unlock(m);
	pc_check(r == 0, "pthread_mutex_unlock() returned %s", rn(r));
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getrobust.004");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
