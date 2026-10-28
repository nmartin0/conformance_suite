/*
 * Permission to search a directory and permission to read it are separate
 * bits of the owner class: search is needed to reach a name through the
 * directory (and to chdir into it), read to open it for listing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.7-file-access-permissions.004
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define U_CHILD 65534

struct row {
	unsigned mode;
	int search;	/* stat("dd/f") and chdir("dd") should work */
	int list;	/* opendir("dd") should work */
};

static const struct row rows[] = {
	{ 0600, 0, 1 }, { 0100, 1, 0 }, { 0500, 1, 1 }, { 0700, 1, 1 },
	{ 0077, 0, 0 }, { 0000, 0, 0 }, { 0400, 0, 1 }, { 0300, 1, 0 },
};

/* In the unprivileged child: bit 1 stat failed unexpectedly, 2 chdir, 4 opendir; 8, 16, 32 for the opposite. */
static int probe(const struct row *r)
{
	struct stat sb;
	DIR *d;
	int code = 0, e;

	if (stat("dd/f", &sb) == 0) {
		if (!r->search)
			code |= 8;
	} else {
		e = errno;
		if (r->search || e != EACCES)
			code |= 1;
	}
	if (chdir("dd") == 0) {
		if (!r->search)
			code |= 16;
		(void)chdir("..");
	} else {
		e = errno;
		if (r->search || e != EACCES)
			code |= 2;
	}
	d = opendir("dd");
	if (d != NULL) {
		if (!r->list)
			code |= 32;
		(void)closedir(d);
	} else {
		e = errno;
		if (r->list || e != EACCES)
			code |= 4;
	}
	return code;
}

static int run(const struct row *r)
{
	pid_t kid = fork();
	int st;

	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(U_CHILD) != 0 || setuid(U_CHILD) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		_exit(probe(r));
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) >= 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

int main(void)
{
	size_t i;
	int c;

	pc_begin("XBD.4.7-file-access-permissions.004");
	pc_make_dir("dd");
	pc_make_file("dd/f", "data");
	if (geteuid() == 0) {
		if (chown("dd", U_CHILD, U_CHILD) != 0 || chown("dd/f", U_CHILD, U_CHILD) != 0 || chmod(".", 0711) != 0)
			pc_unresolved("chown or chmod failed: %s", pc_errname(errno));
	}
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		/* the file is reached through dd/f, so give the owner back full rights to change dd itself */
		if (chmod("dd", rows[i].mode) != 0)
			pc_unresolved("chmod(dd, %o) failed: %s", rows[i].mode, pc_errname(errno));
		c = run(&rows[i]);
		pc_check(c == 0, "directory of mode %04o: result code %d (1 stat dd/f wrong, 2 chdir wrong, 4 opendir wrong; 8, 16, 32 the same when it succeeded but should not); EACCES expected for the refusals",
		    rows[i].mode, c);
	}
	pc_pass();
}
