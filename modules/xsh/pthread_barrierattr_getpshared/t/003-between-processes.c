/*
 * A process-shared barrier in shared memory works between a parent and a child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_barrierattr_getpshared.003
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
	pthread_barrier_t b;
	atomic_int done, pdone;
	int result, presult;
};

static struct shared *shm;

static void *parent_wait(void *arg)
{
	(void)arg;
	shm->presult = pthread_barrier_wait(&shm->b);
	atomic_store(&shm->pdone, 1);
	return NULL;
}

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
	pthread_barrierattr_t a;
	pthread_t pt;
	int fd, i, st, mine;

	pc_begin("XSH.pthread_barrierattr_getpshared.003");
	fd = open("shared", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)sizeof *s) == 0, "ftruncate failed: %s", pc_errname(errno));
	s = mmap(NULL, sizeof *s, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(s != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)close(fd);
	s->result = s->presult = -1;
	atomic_init(&s->done, 0);
	atomic_init(&s->pdone, 0);
	shm = s;
	NEED(pthread_barrierattr_init(&a) == 0, "pthread_barrierattr_init failed");
	NEED(pthread_barrierattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0, "pthread_barrierattr_setpshared failed");
	NEED(pthread_barrier_init(&s->b, &a, 2) == 0, "pthread_barrier_init failed");
	(void)atexit(reap_child);
	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0) {
		s->result = pthread_barrier_wait(&s->b);
		atomic_store(&s->done, 1);
		_exit(0);
	}
	pc_test();
	NEED(pthread_create(&pt, NULL, parent_wait, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !(atomic_load(&s->done) && atomic_load(&s->pdone)); i++)
		sleep_ms(5);
	pc_check(atomic_load(&s->pdone) && atomic_load(&s->done), "the parent and the child did not both leave the barrier within 10 s");
	NEED(pthread_join(pt, NULL) == 0, "pthread_join failed");
	mine = s->presult;
	pc_check((mine == PTHREAD_BARRIER_SERIAL_THREAD && s->result == 0) ||
	    (mine == 0 && s->result == PTHREAD_BARRIER_SERIAL_THREAD),
	    "pthread_barrier_wait() returned %s in the parent and %s in the child, expected one PTHREAD_BARRIER_SERIAL_THREAD and one 0",
	    rn(mine), rn(s->result));
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_barrierattr_getpshared.003");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
