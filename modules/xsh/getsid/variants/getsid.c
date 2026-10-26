/*
 * getsid.c - conforming variants of getsid(), for the variant checks (CI
 * only).
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
 * getsid() ERRORS [EPERM] leaves it to the implementation whether the
 * caller may see that session.
 */
pid_t getsid(pid_t pid)
{
	const char *v = variant_of("getsid.");
	pid_t mine, s;
	REAL(getsid);

	if (IS("deny_other_session") && pid > 0) {
		mine = real_getsid(0);
		s = real_getsid(pid);
		if (s != -1 && s != mine) {
			errno = EPERM;
			return -1;
		}
	}
	return real_getsid(pid);
}
