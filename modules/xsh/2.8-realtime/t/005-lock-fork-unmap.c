/*
 * A memory lock is not inherited by a fork child, and unmapping removes it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.8-realtime.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void on_alarm(int s)
{
	(void)s;
	pc_unresolved("the test timed out");
}

/* 1 when the pages are locked (msync with MS_INVALIDATE fails with EBUSY), 0 when not, -1 on another error. */
static int locked(void *p, size_t len)
{
	int r = msync(p, len, MS_ASYNC | MS_INVALIDATE);

	if (r == -1 && errno == EBUSY)
		return 1;
	return r == 0 ? 0 : -1;
}

int main(void)
{
	long pg = sysconf(_SC_PAGESIZE);
	int fd, st, r;
	char *p, *q;
	pid_t k;

	pc_begin("XSH.2.8-realtime.005");
	NEED(pg > 0, "no page size");
	(void)signal(SIGALRM, on_alarm);
	(void)alarm(30);
	fd = open("f", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, 2 * pg) == 0, "ftruncate failed");
	p = mmap(NULL, (size_t)(2 * pg), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	NEED(locked(p, (size_t)pg) == 0, "the new mapping is already locked, or msync failed");
	pc_test();
	r = mlock(p, (size_t)pg);
	if (r == -1 && (errno == EAGAIN || errno == ENOMEM || errno == EPERM))
		pc_unresolved("mlock() failed: %s (resources or privileges)", pc_errname(errno));
	pc_check(r == 0, "mlock() failed: %s", pc_errname(errno));
	NEED(locked(p, (size_t)pg) == 1, "the page is not seen as locked after mlock()");
	k = fork();
	NEED(k != -1, "fork failed");
	if (k == 0)
		_exit(locked(p, (size_t)pg) == 0 ? 0 : 1);
	NEED(waitpid(k, &st, 0) == k, "waitpid failed");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "in the child the page is locked or msync failed (status %d)", st);
	pc_check(locked(p, (size_t)pg) == 1, "the parent's lock was lost when the child was made");
	NEED(munmap(p, (size_t)pg) == 0, "munmap failed: %s", pc_errname(errno));
	q = mmap(p, (size_t)pg, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
	NEED(q == p, "the mapping could not be placed at the same address again");
	pc_check(locked(q, (size_t)pg) == 0, "a new mapping at the address of an unmapped locked page is locked");
	pc_pass();
}
