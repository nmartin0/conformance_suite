/*
 * dup.c - conforming variants for dup(), dup2() and dup3(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <fcntl.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "open_max_indeterminate": XBD <limits.h> lists {OPEN_MAX} among the
 * runtime invariant values that may be indeterminate, and sysconf() then
 * returns -1 without changing errno. Descriptors are still limited by
 * RLIMIT_NOFILE (getrlimit()).
 */
long sysconf(int name)
{
	const char *v = variant_of("dup.");
	REAL(sysconf);

	if (IS("open_max_indeterminate") && name == _SC_OPEN_MAX)
		return -1;
	return real_sysconf(name);
}

/*
 * "hidden_fds": the implementation keeps descriptors of its own open in
 * the process (dup() APPLICATION USAGE: catalogs, tracing); their numbers
 * are not specified, and XSH 2.6 still allocates the lowest one not open.
 * One sits just above the lowest free descriptor at start-up (leaving a
 * gap below it), one at 300; both close-on-exec, so each new program gets
 * its own.
 */
__attribute__((constructor)) static void hidden_fds(void)
{
	const char *v = variant_of("dup.");
	int n;

	if (!IS("hidden_fds"))
		return;
	n = open("/dev/null", O_RDONLY | O_CLOEXEC);
	if (n < 0)
		return;
	(void)fcntl(n, F_DUPFD_CLOEXEC, n + 1);
	(void)fcntl(n, F_DUPFD_CLOEXEC, 300);
	(void)close(n);
}
