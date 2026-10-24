/*
 * _exit() applies the process's semadj values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.017
 * VARIANT   wait.status_encoding
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

/* semctl()'s fourth argument: the application declares it (XSH semctl()). */
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int main(void)
{
	pc_begin("XSH._Exit.017");
	{
		struct sembuf op;
		int id, st = 0, v;
		pid_t kid, w;

		id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
		pc_check(id != -1, "semget failed: %s", pc_errname(errno));
		if (semctl(id, 0, SETVAL, (union semun){ .val = 5 }) != 0) {
			(void)semctl(id, 0, IPC_RMID);
			pc_unresolved("semctl(SETVAL) failed: %s", pc_errname(errno));
		}
		pc_test();
		kid = fork();
		if (kid == -1) {
			(void)semctl(id, 0, IPC_RMID);
			pc_unresolved("fork failed: %s", pc_errname(errno));
		}
		if (kid == 0) {
			op.sem_num = 0;
			op.sem_op = 1;
			op.sem_flg = SEM_UNDO;
			_exit(semop(id, &op, 1) == 0 ? 0 : 99);
		}
		w = waitpid(kid, &st, 0);
		{
			/* No bound on when termination's consequences are done: up to 10 s. */
			int i;
			for (i = 0; i < 1000; i++) {
				v = semctl(id, 0, GETVAL);
				if (v == 5 || v == -1)
					break;
				sleep_ms(10);
			}
		}
		(void)semctl(id, 0, IPC_RMID);
		if (w != kid || (WIFEXITED(st) && WEXITSTATUS(st) == 99))
			pc_unresolved("the child could not run semop()");
		pc_check(v == 5, "semval is %d 10 s after the child's _exit(), expected 5", v);
	}
	pc_pass();
}
