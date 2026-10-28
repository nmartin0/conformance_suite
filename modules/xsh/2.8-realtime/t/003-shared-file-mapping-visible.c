/*
 * Stores through a shared mapping of a file are seen through the same mapping in another process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.8-realtime.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

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

	pc_begin("XSH.2.8-realtime.003");
	ps = sysconf(_SC_PAGESIZE);
	NEED(ps > 0, "no page size");
	fd = open("f", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, ps) == 0, "ftruncate failed");
	NEED(pipe(go) == 0 && pipe(ready) == 0, "pipe failed");
	p = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	p[0] = 'a';
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(30);
	k = fork();
	NEED(k != -1, "fork failed");
	if (k == 0) {
		/* The child maps the file itself and stores in its own mapping. */
		volatile char *q = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		char z;

		(void)close(go[1]);
		(void)close(ready[0]);
		if (q == MAP_FAILED)
			_exit(2);
		if (read(go[0], &z, 1) != 1)
			_exit(3);
		q[1] = 'b';
		q[ps - 1] = 'e';
		z = 'x';
		if (write(ready[1], &z, 1) != 1)
			_exit(4);
		/* and sees the parent's store from before the fork */
		_exit(q[0] == 'a' ? 0 : 5);
	}
	(void)close(ready[1]);	/* so that a child that ends early shows as end-of-file below */
	NEED(write(go[1], "g", 1) == 1, "write failed");
	NEED(read(ready[0], &c, 1) == 1, "the child did not report");
	pc_check(p[1] == 'b', "the byte the child stored through its own mapping reads %d in the parent", p[1]);
	pc_check(p[ps - 1] == 'e', "the last byte of the page, stored by the child, reads %d in the parent", p[ps - 1]);
	for (i = 0; i < 300; i++) {
		pid_t r = waitpid(k, &st, WNOHANG);
		struct timespec ts = { 0, 10000000 };

		if (r == k)
			break;
		(void)nanosleep(&ts, NULL);
	}
	NEED(i < 300, "the child did not end");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child did not see the parent's store (status %d)", st);
	pc_pass();
}
