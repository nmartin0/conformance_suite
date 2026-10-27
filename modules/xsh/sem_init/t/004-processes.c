/*
 * With pshared non-zero the semaphore is shared between processes: in
 * memory both map, a post in one process releases a wait in the other,
 * both ways.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_init.004
 */
#include "semh.h"
#include <sys/mman.h>

int main(void)
{
	sem_t *s;
	struct timespec t;
	pid_t k;
	long len;
	int fd, r;

	pc_begin("XSH.sem_init.004");
	guard(60);
	len = sysconf(_SC_PAGESIZE);
	NEED(len > 0 && (size_t)len >= 2 * sizeof(sem_t), "page size unsuitable");
	fd = open("pcsem.map", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed");
	NEED(ftruncate(fd, len) == 0, "ftruncate failed");
	s = mmap(NULL, (size_t)len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(s != MAP_FAILED, "mmap failed");
	(void)close(fd);
	(void)unlink("pcsem.map");
	NEED(sem_init(&s[0], 1, 0) == 0, "sem_init(pshared 1) failed: %s", pc_errname(errno));
	NEED(sem_init(&s[1], 1, 0) == 0, "sem_init(pshared 1) failed: %s", pc_errname(errno));
	pc_test();
	k = fork();
	NEED(k >= 0, "fork failed");
	if (k == 0) {
		/* The child waits for the parent's post, then posts the second semaphore. */
		clk_now(CLOCK_REALTIME, &t);
		ts_add_ms(&t, 20000);
		if (sem_timedwait(&s[0], &t) != 0)
			_exit(10);
		if (sem_post(&s[1]) != 0)
			_exit(11);
		_exit(0);
	}
	sleep_ms(100);
	r = sem_post(&s[0]);
	pc_check(r == 0, "sem_post() on the shared semaphore returned %d (%s)", r, pc_errname(errno));
	clk_now(CLOCK_REALTIME, &t);
	ts_add_ms(&t, 20000);
	r = sem_timedwait(&s[1], &t);
	if (r != 0) {
		int e = errno;

		(void)kill(k, SIGKILL);
		(void)reap(k);
		pc_check(0, "the child's post was not seen by the parent: %s", pc_errname(e));
	}
	r = reap(k);
	pc_check(r == 0, "the child's wait or post on the shared semaphore failed (status %d)", r);
	pc_pass();
}
