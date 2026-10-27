/*
 * A new owner that terminates without making the state consistent leaves it inconsistent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_consistent.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
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

static int lock10(pthread_mutex_t *m)
{
	struct timespec ts;

	NEED(clock_gettime(CLOCK_REALTIME, &ts) == 0, "clock_gettime failed");
	ts.tv_sec += 10;
	return pthread_mutex_timedlock(m, &ts);
}

/* Runs fn(m) in a child that terminates holding whatever it locked; its exit status is fn(m). */
static int child_run(pthread_mutex_t *m, int (*fn)(pthread_mutex_t *))
{
	int st;

	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0)
		_exit(fn(m));
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	return WIFEXITED(st) ? WEXITSTATUS(st) : 255;
}

static pthread_mutex_t *make_shared_robust(void)
{
	pthread_mutex_t *m;
	pthread_mutexattr_t a;
	int fd;

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
	(void)pthread_mutexattr_destroy(&a);
	(void)atexit(reap_child);
	return m;
}

static int lock_and_die(pthread_mutex_t *m)
{
	return pthread_mutex_lock(m) == 0 ? 0 : 3;
}


/* Locks the mutex, which must report EOWNERDEAD, and terminates holding it. */
static int lock_dead_and_die(pthread_mutex_t *m)
{
	return lock10(m) == EOWNERDEAD ? 0 : 3;
}

int main(void)
{
	pthread_mutex_t *m;
	int r;

	pc_begin("XSH.pthread_mutex_consistent.003");
	m = make_shared_robust();
	NEED(child_run(m, lock_and_die) == 0, "the first child could not lock the mutex");
	NEED(child_run(m, lock_dead_and_die) == 0, "the second child did not get EOWNERDEAD");
	pc_test();
	r = lock10(m);
	pc_check(r == EOWNERDEAD, "pthread_mutex_timedlock() after the second owner terminated returned %s, expected EOWNERDEAD", rn(r));
	r = pthread_mutex_consistent(m);
	pc_check(r == 0, "pthread_mutex_consistent() returned %s", rn(r));
	r = pthread_mutex_unlock(m);
	pc_check(r == 0, "pthread_mutex_unlock() returned %s", rn(r));
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutex_consistent.003");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
