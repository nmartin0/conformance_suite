/*
 * A child does not inherit semadj values: its exit undoes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.021
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
#include <sys/sem.h>

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
	pc_begin("XSH.fork.021");
	{
		struct sembuf op;
		int id, st, v;
		pid_t kid;

		id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
		pc_check(id != -1, "semget failed: %s", pc_errname(errno));
		op.sem_num = 0;
		op.sem_op = 1;
		op.sem_flg = SEM_UNDO;
		if (semop(id, &op, 1) != 0) {
			(void)semctl(id, 0, IPC_RMID);
			pc_unresolved("semop failed: %s", pc_errname(errno));
		}
		pc_test();
		kid = fork();
		if (kid == -1) {
			(void)semctl(id, 0, IPC_RMID);
			pc_unresolved("fork failed: %s", pc_errname(errno));
		}
		if (kid == 0)
			_exit(0);
		(void)waitpid(kid, &st, 0);
		v = semctl(id, 0, GETVAL);
		(void)semctl(id, 0, IPC_RMID);
		pc_check(v == 1, "semval is %d after the child's exit, expected 1", v);
	}
	pc_pass();
}
