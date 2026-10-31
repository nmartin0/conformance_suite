/*
 * pcgroups.c - drop supplementary groups where the system can (not POSIX).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * setgroups() is not in POSIX, so this file is built apart from pc.c,
 * without the strict feature macros, so that the system's own headers
 * declare it with their own prototype. The driver builds it once; if that
 * fails (no setgroups(), or no declaration), it builds it again with
 * PC_NO_SETGROUPS, and pc_drop_groups() does nothing. Either way the tests
 * then check with getgroups() that no group but their own is left, and are
 * UNRESOLVED otherwise. No operating system is named anywhere.
 */
#ifndef PC_NO_SETGROUPS
#include <grp.h>
#include <sys/types.h>
#include <unistd.h>
#endif

int pc_drop_groups(void);
int pc_set_one_group(long g);

/* Leave only group 65534 as a supplementary group: 0 on success, -1 if not done. */
int pc_drop_groups(void)
{
#ifndef PC_NO_SETGROUPS
	gid_t g = 65534;

	return setgroups(1, &g) == 0 ? 0 : -1;
#else
	return -1;
#endif
}

/*
 * Make g the only supplementary group (pcexec, before it changes user):
 * 0 on success, -1 on failure, -2 where the system has no setgroups().
 */
int pc_set_one_group(long g)
{
#ifndef PC_NO_SETGROUPS
	gid_t one = (gid_t)g;

	return setgroups(1, &one) == 0 ? 0 : -1;
#else
	(void)g;
	return -2;
#endif
}
