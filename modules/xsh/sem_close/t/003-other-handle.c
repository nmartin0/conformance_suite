/*
 * Closing one of two handles of a semaphore leaves the other usable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_close.003
 */
#include "semh.h"

int main(void)
{
	const char *n;
	sem_t *a, *b;

	pc_begin("XSH.sem_close.003");
	guard(30);
	n = sn_make("a");
	a = sn_create(n, 1);
	b = sem_open(n, 0);
	NEED(b != SEM_FAILED, "second sem_open failed: %s", pc_errname(errno));
	if (a == b) {
		(void)sem_close(a);
		pc_notinuse("sem_open() returned the same handle twice: closing one closes both");
	}
	pc_test();
	pc_check(sem_close(a) == 0, "sem_close() of the first handle failed");
	pc_check(sem_post(b) == 0, "sem_post() through the other handle failed (%s)", pc_errname(errno));
	expect_count(b, 2, "through the other handle after the first was closed");
	pc_check(sem_close(b) == 0, "sem_close() of the second handle failed");
	pc_pass();
}
