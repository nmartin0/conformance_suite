/*
 * shm_open() fails with EMFILE when every descriptor the process may have
 * is open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.shm_open.014
 * MUTANT    shm_open.emfile_as_enfile
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* The page size, from sysconf(_SC_PAGESIZE): never assumed. */
static long pg;

static void get_page_size(void)
{
	pg = sysconf(_SC_PAGESIZE);
	if (pg <= 0)
		pc_unresolved("sysconf(_SC_PAGESIZE) gave %ld", pg);
}

/*
 * Shared memory object names for this run: "/pc<pid><tag>", unique per
 * process, with at most 14 bytes after the slash ({_POSIX_NAME_MAX}) for a
 * tag of up to two characters. Every name made is removed at exit (in the
 * parent; children leave with _exit()).
 */
static char shm_names[8][64];
static int shm_nnames;
static pid_t shm_owner;

static void shm_cleanup(void)
{
	int i;

	if (getpid() != shm_owner)
		return;
	for (i = 0; i < shm_nnames; i++)
		(void)shm_unlink(shm_names[i]);
}

static const char *shm_name(const char *tag)
{
	char *n;

	if (shm_nnames == 8)
		pc_unresolved("too many shared memory object names");
	if (shm_nnames == 0) {
		shm_owner = getpid();
		if (atexit(shm_cleanup) != 0)
			pc_unresolved("atexit failed");
	}
	n = shm_names[shm_nnames++];
	snprintf(n, sizeof shm_names[0], "/pc%ld%s", (long)getpid(), tag);
	(void)shm_unlink(n);	/* a leftover from an earlier process with this ID */
	return n;
}

/* A new shared memory object of size bytes, open O_RDWR (setup). */
static int new_object(const char *name, long size, mode_t mode)
{
	int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, mode);

	if (fd < 0)
		pc_unresolved("shm_open(%s, O_RDWR|O_CREAT|O_EXCL) failed: %s", name, pc_errname(errno));
	if (size > 0 && ftruncate(fd, (off_t)size) != 0)
		pc_unresolved("ftruncate of %s failed: %s", name, pc_errname(errno));
	return fd;
}

/* The status of the object open on fd (UNRESOLVED if fstat() fails). */
static struct stat fd_stat(int fd)
{
	struct stat st;

	if (fstat(fd, &st) != 0)
		pc_unresolved("fstat failed: %s", pc_errname(errno));
	return st;
}

/* A MAP_SHARED mapping of len bytes of fd with prot (UNRESOLVED on failure). */
static unsigned char *map(int fd, long len, int prot)
{
	void *p = mmap(NULL, (size_t)len, prot, MAP_SHARED, fd, 0);

	if (p == MAP_FAILED)
		pc_unresolved("mmap of a shared memory object failed: %s", pc_errname(errno));
	return p;
}

/*
 * Setup for unprivileged(): a file no one may read, which the child can
 * open only with appropriate privileges.
 */
static void pc_searchable(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
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
 * Run fn in a child that has no appropriate privileges and another user ID
 * than the caller: when the test runs as root, the child takes user and
 * group 65534; otherwise the test is UNRESOLVED (it needs a second user).
 * Returns what fn returns (0 to 96); a child that cannot drop privileges,
 * still has them (it can read the mode 0 file .pc-priv), or dies makes the
 * test UNRESOLVED.
 */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	if (geteuid() != 0)
		pc_unresolved("needs to run as root, to have a second user ID");
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		(void)pc_drop_groups();	/* core/pcgroups.c: setgroups() where the system has it */
		if (setgid(65534) != 0 || setuid(65534) != 0 || geteuid() == 0)
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
		pc_unresolved("the unprivileged child kept supplementary groups (no way to drop them here)");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

/* Wait for a child and return its exit status; UNRESOLVED if it did not exit. */
static int reap(pid_t kid)
{
	int st;

	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st))
		pc_unresolved("a child did not exit (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

int main(void)
{
	pc_begin("XSH.shm_open.014");
	{
		struct rlimit rl;
		const char *name;
		int fd, lim, i;

		name = shm_name("a");
		(void)close(new_object(name, 0, 0600));
		fd = open(".", O_RDONLY);
		pc_check(fd >= 0, "cannot open .");
		lim = pc_lowest_free_fd() + 4;
		if (lim < 20)
			lim = 20;	/* not below {_POSIX_OPEN_MAX} (Q-42) */
		for (i = 0; i < 1024; i++)
			if (fcntl(i, F_GETFD) != -1 && i + 1 > lim)
				lim = i + 1;
		if (lim > 256)
			pc_unresolved("a descriptor near %d is open", lim);
		for (i = 0; i < lim; i++)
			if (fcntl(i, F_GETFD) == -1)
				pc_check(dup2(fd, i) == i, "cannot fill slot %d", i);
		pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
		rl.rlim_cur = (rlim_t)lim;
		pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
		if (getrlimit(RLIMIT_NOFILE, &rl) != 0 || rl.rlim_cur != (rlim_t)lim)
			pc_unresolved("RLIMIT_NOFILE did not take the value %d", lim);
		pc_test();
		errno = 0;
		pc_expect_err(shm_open(name, O_RDWR, 0), EMFILE, "shm_open() with every allowed descriptor open");
	}
	pc_pass();
}
