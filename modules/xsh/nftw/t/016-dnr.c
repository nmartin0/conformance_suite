/*
 * An unreadable directory is reported as FTW_DNR and not entered.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.016
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXREC 64

struct rec {
	char path[160];
	int type, level, base;
	struct stat sb;
};

static struct rec rec[MAXREC];
static int nrec;

static int record(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	if (nrec < MAXREC) {
		struct rec *r = &rec[nrec];

		strncpy(r->path, p, sizeof r->path - 1);
		r->path[sizeof r->path - 1] = '\0';
		r->type = t;
		r->level = f->level;
		r->base = f->base;
		if (t != FTW_NS)
			r->sb = *sb;
	}
	nrec++;
	return 0;
}

/* The object's own name: the part of the pathname at base. */
static const char *leaf(const struct rec *r)
{
	if (r->base < 0 || (size_t)r->base > strlen(r->path))
		return "";
	return r->path + r->base;
}

static int count(const char *name)
{
	int i, n = 0;

	for (i = 0; i < nrec && i < MAXREC; i++)
		if (strcmp(leaf(&rec[i]), name) == 0)
			n++;
	return n;
}

/* Index of the one record named name; -1 if none, -2 if several. */
static int idx(const char *name)
{
	int i, found = -1;

	for (i = 0; i < nrec && i < MAXREC; i++)
		if (strcmp(leaf(&rec[i]), name) == 0) {
			if (found != -1)
				return -2;
			found = i;
		}
	return found;
}


/*
 * Setup for unprivileged(): when run as root, make the working directory
 * searchable for the child (relative paths need nothing above it). It is
 * called right after pc_begin(), in the setup phase, so no planted fault
 * applies to it.
 */
static void pc_searchable(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	/* A file no one may read: the child can open it only with privileges. */
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

/*
 * Supplementary groups: a child that drops root keeps root's supplementary
 * groups, which would put it in the group class of files root created
 * (XBD 4.5). setgroups() is not POSIX: pc_drop_groups() uses it where the
 * system has it (found when the suite is built); either way the child must
 * then have no group but its own, or the test is UNRESOLVED.
 */
static void drop_groups(void)
{
	(void)pc_drop_groups();	/* core/pcgroups.c: setgroups() where the system has it */
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
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534 (the working directory is
 * made searchable for it; relative paths need nothing above it). Returns
 * what fn returns (0 to 96); a child that cannot drop privileges, that
 * still has them (it can read the mode 0 file .pc-priv, as with a
 * capability), or that dies, makes the test UNRESOLVED.
 */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		if (!groups_dropped())
			_exit(98);
		{
			int pfd = open(".pc-priv", O_RDONLY);
			if (pfd >= 0)
				_exit(97);
		}
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges (it can read a mode 0 file)");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups (no way to drop them here)");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static int child(void)
{
	int r = nftw("t", record, 10, 0), i;

	if (r != 0)
		return 1;
	if (nrec != 3)
		return 2;
	i = idx("u");
	if (i < 0 || rec[i].type != FTW_DNR)
		return 3;
	if (idx("a") < 0 || idx("t") < 0)
		return 4;
	if (count("z") != 0)
		return 5;
	return 0;
}

int main(void)
{
	int rc;

	pc_begin("XSH.nftw.016");
	pc_searchable();
	pc_make_dir("t");
	pc_make_file("t/a", "hello");
	pc_make_dir("t/u");
	pc_make_file("t/u/z", "z");
	pc_check(chmod("t/u", 0) == 0, "chmod failed");
	pc_test();
	rc = unprivileged(child);
	/* restore first, so the driver can remove the tree after a FAIL */
	if (chmod("t/u", 0700) != 0)
		pc_unresolved("chmod failed");
	switch (rc) {
	case 0:
		break;
	case 1:
		pc_fail("nftw() did not return 0");
	case 2:
		pc_fail("fn was not called exactly for t, a and u");
	case 3:
		pc_fail("the unreadable directory was not reported with FTW_DNR");
	case 4:
		pc_fail("t or a was not reported");
	default:
		pc_fail("fn was called for something below the unreadable directory");
	}
	pc_pass();
}
