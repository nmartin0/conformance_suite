/*
 * A name far beyond the path limits: sem_open() either fails (with
 * ENAMETOOLONG, or another error the page allows for names) or the
 * system has no such limit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.008
 */
#include "semh.h"

#define LEN 5000

int main(void)
{
	static char name[LEN + 1];
	sem_t *a;
	int e;

	pc_begin("XSH.sem_open.008");
	guard(30);
	memset(name, 'x', LEN);
	name[0] = '/';
	name[LEN] = '\0';
	pc_test();
	errno = 0;
	a = sem_open(name, O_CREAT | O_EXCL, (mode_t)0600, 0u);
	e = errno;
	if (a != SEM_FAILED) {
		(void)sem_close(a);
		(void)sem_unlink(name);
		pc_notinuse("the system accepted a name of %d bytes: it has no limit that long", LEN);
	}
	pc_check(e == ENAMETOOLONG || e == EINVAL || e == ENOMEM || e == ENOSPC,
	    "sem_open() of a %d byte name failed with %s, expected ENAMETOOLONG", LEN, pc_errname(e));
	pc_pass();
}
