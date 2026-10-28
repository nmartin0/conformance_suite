/*
 * open() of a file that the caller owns: only the owner permission bits count, even when the group and other bits
 * would allow more.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.7-file-access-permissions.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define U_CHILD 65534
#define U_OTHER 65533

struct row {
	unsigned mode;
	int flags;
	const char *fname;	/* O_RDONLY, O_WRONLY or O_RDWR */
	int allowed;
};

static const struct row rows[] = {
	{ 00077, O_RDONLY, "O_RDONLY", 0 },
	{ 00077, O_WRONLY, "O_WRONLY", 0 },
	{ 00077, O_RDWR, "O_RDWR", 0 },
	{ 00400, O_RDONLY, "O_RDONLY", 1 },
	{ 00400, O_WRONLY, "O_WRONLY", 0 },
	{ 00400, O_RDWR, "O_RDWR", 0 },
	{ 00200, O_RDONLY, "O_RDONLY", 0 },
	{ 00200, O_WRONLY, "O_WRONLY", 1 },
	{ 00200, O_RDWR, "O_RDWR", 0 },
	{ 00600, O_RDONLY, "O_RDONLY", 1 },
	{ 00600, O_WRONLY, "O_WRONLY", 1 },
	{ 00600, O_RDWR, "O_RDWR", 1 },
	{ 00000, O_RDONLY, "O_RDONLY", 0 },
	{ 00000, O_WRONLY, "O_WRONLY", 0 },
	{ 00000, O_RDWR, "O_RDWR", 0 },
	{ 00466, O_WRONLY, "O_WRONLY", 0 },
	{ 00266, O_RDONLY, "O_RDONLY", 0 },
};

/* Opens pf in a child with no privilege; 0 if it opened, the errno if not, -1 if the child could not run. */
static int try_open(int flags)
{
	int pp[2], st;
	pid_t kid;
	int code = -1;

	if (pipe(pp) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		int fd;
		unsigned char res;

		(void)close(pp[0]);
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(U_CHILD) != 0 || setuid(U_CHILD) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		fd = open("pf", flags);
		res = (unsigned char)(fd >= 0 ? 0 : (errno < 200 ? errno : 200));
		if (write(pp[1], &res, 1) != 1)
			_exit(98);
		_exit(0);
	}
	(void)close(pp[1]);
	{
		unsigned char res;

		if (read(pp[0], &res, 1) == 1)
			code = res;
	}
	(void)close(pp[0]);
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	return code;
}

int main(void)
{
	size_t i;
	int r, root = geteuid() == 0;

	pc_begin("XBD.4.7-file-access-permissions.001");
	pc_make_file("pf", "data");
	if (root && chown("pf", U_CHILD, U_CHILD) != 0)
		pc_unresolved("chown failed: %s", pc_errname(errno));
	if (root && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		if (chmod("pf", rows[i].mode) != 0)
			pc_unresolved("chmod(pf, %o) failed: %s", rows[i].mode, pc_errname(errno));
		r = try_open(rows[i].flags);
		if (r < 0)
			pc_unresolved("the unprivileged child could not run");
		if (rows[i].allowed)
			pc_check(r == 0, "open(%s) of a file of mode %04o failed with %s, expected success", rows[i].fname, rows[i].mode, pc_errname(r));
		else
			pc_check(r == EACCES, "open(%s) of a file of mode %04o %s, expected EACCES", rows[i].fname,
			    rows[i].mode, r == 0 ? "succeeded" : pc_errname(r));
	}
	(void)root;
	pc_pass();
}
