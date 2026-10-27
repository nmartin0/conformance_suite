/*
 * sem_unlink() does not wait for another process that has the semaphore
 * open; that process keeps a working semaphore with its value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.006
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a;
	pid_t k;
	int up[2], go[2], r;
	char c = 'x';

	pc_begin("XSH.sem_unlink.006");
	guard(40);
	n = sn_make("a");
	a = sn_create(n, 1);
	NEED(pipe(up) == 0 && pipe(go) == 0, "pipe failed");
	k = fork();
	NEED(k >= 0, "fork failed");
	if (k == 0) {
		sem_t *s = sem_open(n, 0);

		/* Keep only the ends this side uses, so that a dead peer shows as end of file. */
		(void)close(up[0]);
		(void)close(go[1]);

		if (s == SEM_FAILED)
			_exit(10);
		if (write(up[1], &c, 1) != 1)
			_exit(11);
		if (read(go[0], &c, 1) != 1)
			_exit(12);
		/* The name is gone by now; the open semaphore still holds its value. */
		if (sem_trywait(s) != 0)
			_exit(13);
		_exit(sem_trywait(s) == -1 && errno == EAGAIN ? 0 : 14);
	}
	(void)close(up[1]);
	(void)close(go[0]);
	NEED(read(up[0], &c, 1) == 1, "the child did not open the semaphore");
	pc_test();
	r = sem_unlink(n);
	pc_check(r == 0, "sem_unlink() with another process holding the semaphore returned %d (%s)", r,
	    r == -1 ? pc_errname(errno) : "-");
	NEED(write(go[1], &c, 1) == 1, "write failed");
	r = reap(k);
	pc_check(r == 0, "the child's open semaphore was affected by sem_unlink() (status %d)", r);
	(void)sem_close(a);
	pc_pass();
}
