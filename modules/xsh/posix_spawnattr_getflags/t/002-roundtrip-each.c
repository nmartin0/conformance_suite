/*
 * For each flag the header defines, posix_spawnattr_setflags() with that flag alone...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getflags.002
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
	pc_begin("XSH.posix_spawnattr_getflags.002");

	posix_spawnattr_t at;
	short all[8];
	const char *name[8];
	int ps[8] = { 0 };
	int n = 0, i, r;

	all[n] = POSIX_SPAWN_RESETIDS; name[n++] = "RESETIDS";
	all[n] = POSIX_SPAWN_SETPGROUP; name[n++] = "SETPGROUP";
#ifdef POSIX_SPAWN_SETSCHEDPARAM
	ps[n] = 1; all[n] = POSIX_SPAWN_SETSCHEDPARAM; name[n++] = "SETSCHEDPARAM";
#endif
#ifdef POSIX_SPAWN_SETSCHEDULER
	ps[n] = 1; all[n] = POSIX_SPAWN_SETSCHEDULER; name[n++] = "SETSCHEDULER";
#endif
#ifdef POSIX_SPAWN_SETSID
	all[n] = POSIX_SPAWN_SETSID; name[n++] = "SETSID";
#endif
	all[n] = POSIX_SPAWN_SETSIGDEF; name[n++] = "SETSIGDEF";
	all[n] = POSIX_SPAWN_SETSIGMASK; name[n++] = "SETSIGMASK";
	pc_check(posix_spawnattr_init(&at) == 0, "posix_spawnattr_init failed");
	pc_test();
	for (i = 0; i < n; i++) {
		short got = 0x5a;

		r = posix_spawnattr_setflags(&at, all[i]);
		/* The scheduling flags belong to [PS]; a system without it may refuse a flag it declares (EINVAL may-fail). */
		if (ps[i] && r == EINVAL)
			continue;
		pc_check(r == 0, "setflags(POSIX_SPAWN_%s) returned %s", name[i], pc_errname(r));
		r = posix_spawnattr_getflags(&at, &got);
		pc_check(r == 0, "getflags() returned %s", pc_errname(r));
		pc_check(got == all[i], "after setting POSIX_SPAWN_%s the flags are %#x, expected %#x", name[i], got, all[i]);
	}
	(void)posix_spawnattr_destroy(&at);
	pc_pass();
}
