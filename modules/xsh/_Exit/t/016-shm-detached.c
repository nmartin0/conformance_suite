/*
 * _exit() detaches XSI shared memory: shm_nattch drops by one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.016
 * MUTANT    _Exit.lingers
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

int main(void)
{
	pc_begin("XSH._Exit.016");
	{
		struct shmid_ds ds;
		int id, p[2], g[2], st;
		pid_t kid;
		char c;

		pc_check(pipe(p) == 0 && pipe(g) == 0, "pipe failed");
		id = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0600);
		pc_check(id != -1, "shmget failed: %s", pc_errname(errno));
		if (shmat(id, NULL, 0) == (void *)-1) {
			(void)shmctl(id, IPC_RMID, NULL);
			pc_unresolved("shmat failed: %s", pc_errname(errno));
		}
		pc_test();
		kid = fork();
		if (kid == -1) {
			(void)shmctl(id, IPC_RMID, NULL);
			pc_unresolved("fork failed: %s", pc_errname(errno));
		}
		if (kid == 0) {
			(void)write(p[1], "a", 1);
			(void)read(g[0], &c, 1);
			_exit(0);
		}
		if (read(p[0], &c, 1) != 1 || shmctl(id, IPC_STAT, &ds) != 0 || ds.shm_nattch != 2) {
			(void)kill(kid, SIGKILL);
			(void)shmctl(id, IPC_RMID, NULL);
			pc_unresolved("the segment is not attached twice (shm_nattch %lu)", (unsigned long)ds.shm_nattch);
		}
		(void)write(g[1], "g", 1);
		(void)waitpid(kid, &st, 0);
		sleep_ms(50);
		st = shmctl(id, IPC_STAT, &ds);
		(void)shmctl(id, IPC_RMID, NULL);
		pc_check(st == 0, "shmctl(IPC_STAT) failed: %s", pc_errname(errno));
		pc_check(ds.shm_nattch == 1, "shm_nattch is %lu after the child's _exit(), expected 1", (unsigned long)ds.shm_nattch);
	}
	pc_pass();
}
