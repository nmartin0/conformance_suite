/*
 * A process-shared spin lock in shared memory works between a parent and a child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_spin_init.002
 */
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

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

#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED >= 0
struct shared {
	pthread_spinlock_t s;
	atomic_int step;	/* child sets 1 after its first try, 2 after its second */
	atomic_int res1;
	atomic_int res2;
	atomic_int go;		/* parent sets 1 after the unlock */
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

static int wait_for(atomic_int *f, int v, long ms)
{
	long w = 0;

	while (atomic_load(f) < v && w < ms) {
		sleep_ms(5);
		w += 5;
	}
	return atomic_load(f) >= v;
}

int main(void)
{
	struct shared *sh;
	int fd, r, st;

	pc_begin("XSH.pthread_spin_init.002");
	fd = open("shared", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)sizeof *sh) == 0, "ftruncate failed: %s", pc_errname(errno));
	sh = mmap(NULL, sizeof *sh, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(sh != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)close(fd);
	atomic_init(&sh->step, 0);
	atomic_init(&sh->res1, -1);
	atomic_init(&sh->res2, -1);
	atomic_init(&sh->go, 0);
	r = pthread_spin_init(&sh->s, PTHREAD_PROCESS_SHARED);
	NEED(r == 0, "pthread_spin_init(PTHREAD_PROCESS_SHARED) failed: %s", rn(r));
	NEED(pthread_spin_lock(&sh->s) == 0, "pthread_spin_lock failed");
	(void)atexit(reap_child);
	child_pid = fork();
	NEED(child_pid != -1, "fork failed: %s", pc_errname(errno));
	if (child_pid == 0) {
		int t = pthread_spin_trylock(&sh->s);

		if (t == 0)
			(void)pthread_spin_unlock(&sh->s);
		atomic_store(&sh->res1, t);
		atomic_store(&sh->step, 1);
		while (!atomic_load(&sh->go))
			sleep_ms(2);
		t = pthread_spin_trylock(&sh->s);
		if (t == 0)
			(void)pthread_spin_unlock(&sh->s);
		atomic_store(&sh->res2, t);
		atomic_store(&sh->step, 2);
		_exit(0);
	}
	pc_test();
	NEED(wait_for(&sh->step, 1, 10000), "the child did not run");
	r = atomic_load(&sh->res1);
	pc_check(r == EBUSY, "in the child, pthread_spin_trylock() of the lock held by the parent returned %s, expected EBUSY", rn(r));
	r = pthread_spin_unlock(&sh->s);
	pc_check(r == 0, "pthread_spin_unlock() in the parent returned %s", rn(r));
	atomic_store(&sh->go, 1);
	pc_check(wait_for(&sh->step, 2, 10000), "the child did not finish within 10 s");
	r = atomic_load(&sh->res2);
	pc_check(r == 0, "in the child, pthread_spin_trylock() after the parent unlocked returned %s, expected 0", rn(r));
	while (waitpid(child_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	child_pid = -1;
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_spin_init.002");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
