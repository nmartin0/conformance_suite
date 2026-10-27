/*
 * A name far beyond the path limits: if sem_open() took it, sem_unlink()
 * must remove it without ENAMETOOLONG; if not, sem_unlink() fails (the
 * name does not exist; ENAMETOOLONG is allowed too).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.007
 */
#include "semh.h"

#define LEN 5000

int main(void)
{
	static char name[LEN + 1];
	sem_t *a;
	int r;

	pc_begin("XSH.sem_unlink.007");
	guard(30);
	memset(name, 'y', LEN);
	name[0] = '/';
	name[LEN] = '\0';
	pc_test();
	a = sem_open(name, O_CREAT | O_EXCL, (mode_t)0600, 0u);
	if (a != SEM_FAILED) {
		(void)sem_close(a);
		errno = 0;
		r = sem_unlink(name);
		pc_check(r == 0, "sem_unlink() of a name sem_open() accepted returned %d (%s), expected 0", r,
		    pc_errname(errno));
		pc_pass();
	}
	errno = 0;
	r = sem_unlink(name);
	pc_check(r == -1 && (errno == ENAMETOOLONG || errno == ENOENT),
	    "sem_unlink() of a long name that sem_open() refused returned %d (%s), expected -1 with ENAMETOOLONG or ENOENT",
	    r, pc_errname(errno));
	pc_pass();
}
