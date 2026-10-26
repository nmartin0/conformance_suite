/*
 * getpgid.c - conforming variants of getpgid(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "deny_other_session": EPERM for a process in another session. XSH
 * getpgid() ERRORS [EPERM] leaves it to the implementation whether such a
 * process may be asked.
 * "einval_neg": EINVAL for a negative pid, the may-fail error for an
 * invalid pid (ERRORS [EINVAL]).
 */
pid_t getpgid(pid_t pid)
{
	const char *v = variant_of("getpgid.");
	REAL(getpgid);

	if (IS("deny_other_session") && pid > 0) {
		pid_t s = getsid(pid);

		if (s != -1 && s != getsid(0)) {
			errno = EPERM;
			return -1;
		}
	}
	if (IS("einval_neg") && pid < 0) {
		errno = EINVAL;
		return -1;
	}
	return real_getpgid(pid);
}
