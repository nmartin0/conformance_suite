/*
 * pthread_sigmask.c - conforming variants of pthread_sigmask() and
 * sigprocmask(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include "pcvariant.h"

/*
 * "how_values": the how constants are 1, -12345 and 3 (the header half is
 * pthread_sigmask.how_values.h). XBD <signal.h> fixes only their names.
 * This system's values are mapped to the C library's; any other value,
 * the library's own included, is invalid, which XSH pthread_sigmask()
 * requires to fail with EINVAL when set is not null and to be ignored
 * when set is null.
 */
static int map_how(int how, int *real_how)
{
	switch (how) {
	case 1: *real_how = SIG_BLOCK; return 0;
	case -12345: *real_how = SIG_UNBLOCK; return 0;
	case 3: *real_how = SIG_SETMASK; return 0;
	}
	return -1;
}

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
	const char *v = variant_of("pthread_sigmask.");
	int h;
	REAL(pthread_sigmask);

	if (!IS("how_values"))
		return real_pthread_sigmask(how, set, oset);
	if (set == NULL)
		return real_pthread_sigmask(SIG_BLOCK, NULL, oset);
	if (map_how(how, &h) != 0)
		return EINVAL;
	return real_pthread_sigmask(h, set, oset);
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	const char *v = variant_of("pthread_sigmask.");
	int h;
	REAL(sigprocmask);

	if (!IS("how_values"))
		return real_sigprocmask(how, set, oset);
	if (set == NULL)
		return real_sigprocmask(SIG_BLOCK, NULL, oset);
	if (map_how(how, &h) != 0) {
		errno = EINVAL;
		return -1;
	}
	return real_sigprocmask(h, set, oset);
}
