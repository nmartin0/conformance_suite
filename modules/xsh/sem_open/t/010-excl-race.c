/*
 * Six processes racing to create one name with O_CREAT|O_EXCL: exactly
 * one creates it, the others get EEXIST. Three rounds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_open.010
 */
#include "semh.h"

#define NK 6

int main(void)
{
	static const char *tags[3] = { "a", "b", "c" };
	pid_t kids[NK];
	int round, i, p[2], won, lost, st;
	char c;

	pc_begin("XSH.sem_open.010");
	guard(50);
	pc_test();
	for (round = 0; round < 3; round++) {
		const char *n = sn_make(tags[round]);

		NEED(pipe(p) == 0, "pipe failed");
		for (i = 0; i < NK; i++) {
			kids[i] = fork();
			NEED(kids[i] >= 0, "fork failed");
			if (kids[i] == 0) {
				sem_t *s;

				(void)close(p[1]);
				/* Wait for the start: end of file on the pipe. */
				while (read(p[0], &c, 1) > 0)
					;
				errno = 0;
				s = sem_open(n, O_CREAT | O_EXCL, (mode_t)0600, 0u);
				if (s != SEM_FAILED)
					_exit(0);
				_exit(errno == EEXIST ? 1 : 2);
			}
		}
		(void)close(p[0]);
		(void)close(p[1]);
		won = lost = 0;
		for (i = 0; i < NK; i++) {
			st = reap(kids[i]);
			if (st == 0)
				won++;
			else if (st == 1)
				lost++;
			else
				pc_check(0, "a racing sem_open() failed with something other than EEXIST");
		}
		pc_check(won == 1 && lost == NK - 1, "round %d: %d processes created the semaphore, %d got EEXIST; expected 1 and %d",
		    round, won, lost, NK - 1);
	}
	pc_pass();
}
