/*
 * A semaphore made with mode 0 cannot be opened by another open that
 * wants to read it: sem_open() fails with EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.009
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a;
	pid_t k;
	int r;

	pc_begin("XSH.sem_open.009");
	guard(30);
	n = sn_make("a");
	a = sem_open(n, O_CREAT | O_EXCL, (mode_t)0, 1u);
	NEED(a != SEM_FAILED, "sem_open(O_CREAT|O_EXCL, mode 0) failed: %s", pc_errname(errno));
	pc_test();
	k = fork();
	NEED(k >= 0, "fork failed");
	if (k == 0) {
		sem_t *b;

		/* As the owner could change nothing, run as another user when privileged. */
		if (getuid() == 0 || geteuid() == 0) {
			if (setgid(65534) != 0 || setuid(65534) != 0 || geteuid() == 0)
				_exit(20);
		}
		errno = 0;
		b = sem_open(n, 0);
		if (b != SEM_FAILED)
			_exit(2);
		_exit(errno == EACCES ? 0 : 1);
	}
	r = reap(k);
	NEED(r != 20, "privilege could not be dropped in the child");
	pc_check(r != 2, "sem_open() of a semaphore with mode 0 succeeded for a process with no permission");
	pc_check(r == 0, "sem_open() of a semaphore with mode 0 failed with an error other than EACCES");
	(void)sem_close(a);
	pc_pass();
}
