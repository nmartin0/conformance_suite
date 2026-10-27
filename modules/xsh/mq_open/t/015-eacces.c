/*
 * mq_open() of a queue whose permissions deny the access fails with EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.015
 */
#include "mqh.h"
#include <sys/stat.h>

static const char *name;

/* In the unprivileged child: 0 for EACCES, 1 if it opened, 2 + errno otherwise. */
static int try_open(void)
{
	mqd_t q = mq_open(name, O_RDONLY);

	if (q != BADQ)
		return 1;
	return errno == EACCES ? 0 : 2 + (errno < 90 ? errno : 90);
}

/* After dropping privileges: no supplementary group but the child's own. */
static int groups_dropped(void)
{
	gid_t gs[64];
	int n = getgroups(64, gs), i;

	if (n < 0)
		return 0;
	for (i = 0; i < n; i++)
		if (gs[i] != getegid() && gs[i] != getgid())
			return 0;
	return 1;
}

/*
 * Runs try_open in a child that has another user ID than the caller and no
 * appropriate privileges (user and group 65534 when run as root). A child
 * that cannot drop privileges, still has them (it can read a mode 0 file),
 * or dies makes the test UNRESOLVED.
 */
static int unprivileged(void)
{
	pid_t kid;
	int st;

	if (geteuid() != 0)
		pc_unresolved("needs to run as root, to have a second user ID");
	kid = fork();
	NEED(kid != -1, "fork failed");
	if (kid == 0) {
		(void)pc_drop_groups();
		if (setgid(65534) != 0 || setuid(65534) != 0 || geteuid() == 0)
			_exit(99);
		if (!groups_dropped())
			_exit(98);
		if (open(".pc-priv", O_RDONLY) >= 0)
			_exit(97);
		_exit(try_open());
	}
	while (waitpid(kid, &st, 0) == -1)
		NEED(errno == EINTR, "waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

int main(void)
{
	int r;

	pc_begin("XSH.mq_open.015");
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed");
	pc_make_file(".pc-priv", "");
	NEED(chmod(".pc-priv", 0) == 0, "chmod(.pc-priv) failed");
	name = mq_name("a");
	(void)umask(0);
	(void)mq_new(name, O_RDWR, 4, 64);	/* mode 0600: only its owner may open it */
	pc_test();
	r = unprivileged();
	pc_check(r != 1, "another user without privileges opened a mode 0600 queue for reading");
	pc_check(r == 0, "mq_open(O_RDONLY) by another user failed with %s, not EACCES", pc_errname(r - 2));
	pc_pass();
}
