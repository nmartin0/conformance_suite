/*
 * posix_spawnattr_setschedpolicy() with the calling process's own policy returns 0 and...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getschedpolicy.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A descriptor number that is in range and not open. */
static int free_fd_plain(void)
{
	int f = open("/dev/null", O_RDONLY);

	if (f == -1)
		pc_unresolved("cannot open /dev/null");
	(void)close(f);
	return f;
}

int main(void)
{
	pc_begin("XSH.posix_spawnattr_getschedpolicy.002");

	posix_spawnattr_t a, b;
	const int cand[3] = { SCHED_FIFO, SCHED_RR, SCHED_OTHER };
	int own, got = -12345, i, r, own_ok = 1, nok = 0;

	own = sched_getscheduler(0);
	pc_check(own != -1, "sched_getscheduler failed");
	pc_check(posix_spawnattr_init(&a) == 0 && posix_spawnattr_init(&b) == 0, "posix_spawnattr_init failed");
	pc_test();
	/* Even the process's own policy may be refused as not valid (EINVAL may-fail). */
	r = posix_spawnattr_setschedpolicy(&a, own);
	if (r == EINVAL) {
		own_ok = 0;
	} else {
		pc_check(r == 0, "setschedpolicy(own policy) returned %s", pc_errname(r));
		r = posix_spawnattr_getschedpolicy(&a, &got);
		pc_check(r == 0, "getschedpolicy() returned %s", pc_errname(r));
		pc_check(got == own, "the policy is %d, expected %d", got, own);
		nok++;
	}
	for (i = 0; i < 3; i++) {
		r = posix_spawnattr_setschedpolicy(&b, cand[i]);
		if (r == EINVAL)
			continue;	/* may fail for a value it takes to be invalid */
		pc_check(r == 0, "setschedpolicy(%d) returned %s", cand[i], pc_errname(r));
		got = -12345;
		pc_check(posix_spawnattr_getschedpolicy(&b, &got) == 0, "getschedpolicy failed");
		pc_check(got == cand[i], "the policy is %d, expected %d", got, cand[i]);
		nok++;
		if (own_ok) {
			got = -12345;
			pc_check(posix_spawnattr_getschedpolicy(&a, &got) == 0, "getschedpolicy failed");
			pc_check(got == own, "the first object's policy changed to %d", got);
		}
	}
	if (nok == 0)
		pc_unresolved("setschedpolicy() refused every value tried with EINVAL");
	(void)posix_spawnattr_destroy(&a);
	(void)posix_spawnattr_destroy(&b);
	pc_pass();
}
