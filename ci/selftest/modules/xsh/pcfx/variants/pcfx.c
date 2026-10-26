/*
 * pcfx.c - fixture variants for ci/mutate.sh's self-test (CI only).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Not conforming behaviour: each variant forces one known outcome.
 */
#define _GNU_SOURCE 1
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/* Variants act on getppid(): the harness itself uses getpid(). */
pid_t getppid(void)
{
	static int calls;
	const char *v = variant_of("pcfx.");
	REAL(getppid);

	if (IS("pid_neg"))
		return calls++ == 0 ? real_getppid() : -5;	/* setup passes, the test sees -5 */
	if (IS("changes"))
		return real_getppid() + (calls++ % 2);
	if (IS("pid_same"))
		return real_getppid();	/* no change, so TOLERATED */
	if (IS("ppid_bad"))
		return -1;	/* setup fails: UNRESOLVED, so UNDECIDED */
	return real_getppid();
}
