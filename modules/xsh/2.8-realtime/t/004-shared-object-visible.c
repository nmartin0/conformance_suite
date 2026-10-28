/*
 * Stores through a mapping of a shared memory object are seen through another process's mapping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.8-realtime.004
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static char name[64];
static int made;

static void cleanup(void)
{
	if (made)
		(void)shm_unlink(name);
}

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

int main(void)
{
	long ps;
	int fd, st, i, go[2], ready[2];
	volatile char *p;
	pid_t k;
	char c;

	pc_begin("XSH.2.8-realtime.004");
	ps = sysconf(_SC_PAGESIZE);
	NEED(ps > 0, "no page size");
	(void)snprintf(name, sizeof name, "/pc2.8.004.%ld", (long)getpid());
	fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
	NEED(fd >= 0, "shm_open failed: %s", pc_errname(errno));
	made = 1;
	(void)atexit(cleanup);
	NEED(ftruncate(fd, ps) == 0, "ftruncate failed");
	NEED(pipe(go) == 0 && pipe(ready) == 0, "pipe failed");
	p = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(30);
	k = fork();
	NEED(k != -1, "fork failed");
	if (k == 0) {
		/* The child opens the object by name and maps it afresh. */
		int fd2 = shm_open(name, O_RDWR, 0);
		volatile char *q;
		char z;

		(void)close(go[1]);
		(void)close(ready[0]);
		if (fd2 < 0)
			_exit(2);
		q = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
		if (q == MAP_FAILED)
			_exit(3);
		if (read(go[0], &z, 1) != 1)
			_exit(4);
		q[7] = 'q';
		z = 'x';
		_exit(write(ready[1], &z, 1) == 1 ? 0 : 5);
	}
	(void)close(ready[1]);	/* so that a child that ends early shows as end-of-file below */
	NEED(write(go[1], "g", 1) == 1, "write failed");
	NEED(read(ready[0], &c, 1) == 1, "the child did not report");
	pc_check(p[7] == 'q', "the byte the child stored through its mapping reads %d in the parent", p[7]);
	for (i = 0; i < 300; i++) {
		pid_t r = waitpid(k, &st, WNOHANG);
		struct timespec ts = { 0, 10000000 };

		if (r == k)
			break;
		(void)nanosleep(&ts, NULL);
	}
	NEED(i < 300, "the child did not end");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (status %d)", st);
	pc_pass();
}
