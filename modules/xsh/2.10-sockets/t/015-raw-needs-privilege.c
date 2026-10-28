/*
 * Creating a raw socket requires appropriate privileges.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.015
 *
 * A child without appropriate privileges (as root it takes user and group
 * 65534) calls socket(AF_INET, SOCK_RAW, 0) and with IPPROTO_ICMP; both
 * must fail, with EACCES or EPERM.
 */
#include "pcsk.h"

/* Setup (phase one): the working directory searchable and a probe file nobody can read. */
static void unpriv_setup(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

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

/* Runs fn in a child without privilege; its value (0 to 96), or UNRESOLVED if the child could not drop them. */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		alarm(20);
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		if (!groups_dropped())
			_exit(98);
		if (open(".pc-priv", O_RDONLY) >= 0)
			_exit(97);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges (it can read a mode 0 file)");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static int child(void)
{
	int s = socket(AF_INET, SOCK_RAW, 0), e;

	if (s != -1)
		return 1;
	e = errno;
	if (e != EACCES && e != EPERM)
		return 2;
	s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (s != -1)
		return 3;
	e = errno;
	if (e != EACCES && e != EPERM)
		return 4;
	return 0;
}

int main(void)
{
	int r;

	pc_begin("XSH.2.10-sockets.015");
	guard(60);
	unpriv_setup();
	pc_test();
	r = unprivileged(child);
	pc_check(r != 1 && r != 3, "an unprivileged process made a raw socket (%s)", r == 1 ? "protocol 0" : "IPPROTO_ICMP");
	pc_check(r == 0, "creating a raw socket without privilege failed with an error other than EACCES or EPERM (status %d)", r);
	pc_pass();
}
