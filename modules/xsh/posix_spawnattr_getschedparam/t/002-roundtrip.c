/*
 * posix_spawnattr_setschedparam() with the calling process's own scheduling parameters...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getschedparam.002
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
	pc_begin("XSH.posix_spawnattr_getschedparam.002");

	posix_spawnattr_t a, b;
	struct sched_param own, sp, got;
	int cand[2], nc = 0, i, r, v, own_ok = 1, nok = 0;

	memset(&own, 0, sizeof own);
	pc_check(sched_getparam(0, &own) == 0, "sched_getparam failed");
	if ((v = sched_get_priority_min(SCHED_FIFO)) != -1)
		cand[nc++] = v;
	if ((v = sched_get_priority_max(SCHED_FIFO)) != -1)
		cand[nc++] = v;
	pc_check(posix_spawnattr_init(&a) == 0 && posix_spawnattr_init(&b) == 0, "posix_spawnattr_init failed");
	pc_test();
	/*
	 * The attribute's default policy is unspecified, so even the process's
	 * own parameters may be refused as not valid (EINVAL may-fail); then
	 * only the candidates below are tried.
	 */
	r = posix_spawnattr_setschedparam(&a, &own);
	if (r == EINVAL) {
		own_ok = 0;
	} else {
		pc_check(r == 0, "setschedparam(own parameters) returned %s", pc_errname(r));
		memset(&got, 0, sizeof got);
		got.sched_priority = -12345;
		r = posix_spawnattr_getschedparam(&a, &got);
		pc_check(r == 0, "getschedparam() returned %s", pc_errname(r));
		pc_check(got.sched_priority == own.sched_priority, "the priority is %d, expected %d", got.sched_priority, own.sched_priority);
		nok++;
	}
	for (i = 0; i < nc; i++) {
		memset(&sp, 0, sizeof sp);
		sp.sched_priority = cand[i];
		r = posix_spawnattr_setschedparam(&b, &sp);
		if (r == EINVAL)
			continue;	/* may fail for a value it takes to be invalid */
		pc_check(r == 0, "setschedparam(%d) returned %s", cand[i], pc_errname(r));
		got.sched_priority = -12345;
		pc_check(posix_spawnattr_getschedparam(&b, &got) == 0, "getschedparam failed");
		pc_check(got.sched_priority == cand[i], "the priority is %d, expected %d", got.sched_priority, cand[i]);
		nok++;
		if (own_ok) {
			got.sched_priority = -12345;
			pc_check(posix_spawnattr_getschedparam(&a, &got) == 0, "getschedparam failed");
			pc_check(got.sched_priority == own.sched_priority, "the first object's priority changed to %d", got.sched_priority);
		}
	}
	if (nok == 0)
		pc_unresolved("setschedparam() refused every value tried with EINVAL");
	(void)posix_spawnattr_destroy(&a);
	(void)posix_spawnattr_destroy(&b);
	pc_pass();
}
