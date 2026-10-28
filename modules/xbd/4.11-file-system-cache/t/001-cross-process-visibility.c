/*
 * What one process has done to the file system is at once what another
 * process sees: creations, renames, removals, links, symbolic links, and
 * data written or truncated through another open file description - also
 * for names the other process looked up (and did not find) just before.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.11-file-system-cache.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define NSTEPS 11

static int p2c[2], c2p[2];
static int cfd = -1;	/* the child's own descriptor on "data" */

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

static int isdir(const char *p)
{
	struct stat sb;

	return stat(p, &sb) == 0 && S_ISDIR(sb.st_mode);
}

static int missing(const char *p)
{
	struct stat sb;

	return lstat(p, &sb) == -1 && errno == ENOENT;
}

/* Child: look up the names the next step will create (so a stale "not found" could be remembered). */
static void prime(int step)
{
	struct stat sb;

	switch (step) {
	case 0: (void)stat("f1", &sb); break;
	case 1: (void)stat("f2", &sb); break;
	case 3: (void)stat("dd", &sb); break;
	case 5: (void)lstat("sl", &sb); break;
	case 6: (void)lstat("f4", &sb); break;
	default: break;
	}
}

/* Child: whether the step's effect is visible; 0 if so, else a code. */
static int check(int step)
{
	struct stat sb;
	char b[16];
	ssize_t n;

	switch (step) {
	case 0:	/* f1 created, with data */
		return stat("f1", &sb) == 0 && sb.st_size == 5 ? 0 : 1;
	case 1:	/* f1 renamed to f2 */
		return missing("f1") && stat("f2", &sb) == 0 ? 0 : 2;
	case 2:	/* f2 unlinked */
		return missing("f2") ? 0 : 3;
	case 3:	/* directory dd made */
		return isdir("dd") ? 0 : 4;
	case 4:	/* dd removed */
		return missing("dd") ? 0 : 5;
	case 5:	/* symbolic link */
		n = readlink("sl", b, sizeof b);
		return n == 3 && memcmp(b, "tgt", 3) == 0 ? 0 : 6;
	case 6:	/* hard link: f3 and f4 */
		return stat("f4", &sb) == 0 && sb.st_nlink == 2 ? 0 : 7;
	case 7:	/* data written by the parent at offset 0 */
		n = pread(cfd, b, 4, 0);
		return n == 4 && memcmp(b, "AAAA", 4) == 0 ? 0 : 8;
	case 8:	/* the parent wrote again at the same place, after the first write returned */
		n = pread(cfd, b, 4, 0);
		return n == 4 && memcmp(b, "BBBB", 4) == 0 ? 0 : 9;
	case 9:	/* truncated by the parent */
		return fstat(cfd, &sb) == 0 && sb.st_size == 2 ? 0 : 10;
	default:/* the child writes; the parent will look */
		return pwrite(cfd, "CC", 2, 2) == 2 ? 0 : 11;
	}
}

static int child(void)
{
	char c;
	int step = 0;

	(void)signal(SIGALRM, SIG_DFL);
	(void)alarm(100);
	cfd = open("data", O_RDWR);
	if (cfd < 0)
		return 90;
	for (step = 0; step < NSTEPS; step++) {
		char reply;

		if (read(p2c[0], &c, 1) != 1 || c != 'P')
			return 91;
		prime(step);
		reply = 'k';
		if (write(c2p[1], &reply, 1) != 1)
			return 92;
		if (read(p2c[0], &c, 1) != 1 || c != 'C')
			return 93;
		reply = (char)('A' + check(step));
		if (write(c2p[1], &reply, 1) != 1)
			return 94;
	}
	return 0;
}

/* Parent: tell the child to prime, wait; do the step; tell it to check; return its code. */
static int roundtrip(int step, void (*act)(void))
{
	char c = 'P', reply;

	if (write(p2c[1], &c, 1) != 1 || read(c2p[0], &reply, 1) != 1)
		pc_unresolved("the child stopped answering (step %d)", step);
	act();
	c = 'C';
	if (write(p2c[1], &c, 1) != 1 || read(c2p[0], &reply, 1) != 1)
		pc_unresolved("the child stopped answering (step %d)", step);
	return reply - 'A';
}

static int dfd = -1;

static void a0(void) { int fd = open("f1", O_WRONLY | O_CREAT | O_EXCL, 0600); if (fd < 0 || write(fd, "hello", 5) != 5 || close(fd) != 0) pc_unresolved("step 0 setup failed"); }
static void a1(void) { if (rename("f1", "f2") != 0) pc_unresolved("rename failed: %s", pc_errname(errno)); }
static void a2(void) { if (unlink("f2") != 0) pc_unresolved("unlink failed: %s", pc_errname(errno)); }
static void a3(void) { if (mkdir("dd", 0700) != 0) pc_unresolved("mkdir failed: %s", pc_errname(errno)); }
static void a4(void) { if (rmdir("dd") != 0) pc_unresolved("rmdir failed: %s", pc_errname(errno)); }
static void a5(void) { if (symlink("tgt", "sl") != 0) pc_unresolved("symlink failed: %s", pc_errname(errno)); }
static void a6(void) { if (link("f3", "f4") != 0) pc_unresolved("link failed: %s", pc_errname(errno)); }
static void a7(void) { if (pwrite(dfd, "AAAA", 4, 0) != 4) pc_unresolved("pwrite failed: %s", pc_errname(errno)); }
static void a8(void) { if (pwrite(dfd, "BBBB", 4, 0) != 4) pc_unresolved("pwrite failed: %s", pc_errname(errno)); }
static void a9(void) { if (ftruncate(dfd, 2) != 0) pc_unresolved("ftruncate failed: %s", pc_errname(errno)); }
static void a10(void) { }

int main(void)
{
	static void (*acts[NSTEPS])(void) = { a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 };
	static const char *what[NSTEPS] = { "a new file with data", "a rename", "an unlink", "a mkdir", "an rmdir",
	    "a symbolic link", "a hard link", "data written at offset 0", "a second write to the same place",
	    "a truncation", "data written by the other process" };
	pid_t kid;
	int step, code, st;
	char b[8];

	pc_begin("XBD.4.11-file-system-cache.001");
	(void)signal(SIGALRM, on_alarm);
	pc_make_file("data", "0123456789");
	pc_make_file("f3", "z");
	dfd = open("data", O_RDWR);
	if (dfd < 0 || pipe(p2c) != 0 || pipe(c2p) != 0)
		pc_unresolved("setup failed: %s", pc_errname(errno));
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0)
		_exit(child());
	pc_test();
	(void)alarm(100);
	for (step = 0; step < NSTEPS; step++) {
		code = roundtrip(step, acts[step]);
		pc_check(code == 0, "the other process did not see %s at once (code %d)", what[step], code);
	}
	pc_check(waitpid(kid, &st, 0) == kid && WIFEXITED(st) && WEXITSTATUS(st) == 0, "the other process ended with status %#x", (unsigned)st);
	pc_check(pread(dfd, b, 4, 0) == 4 && memcmp(b, "BBCC", 4) == 0, "the data written by the other process is not what this one reads back");
	pc_pass();
}
