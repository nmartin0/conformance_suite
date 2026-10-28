/*
 * For a process with appropriate privileges, execute permission is granted
 * when at least one of the execute bits (owner, group or other) is set, and
 * refused when none is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.7-file-access-permissions.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define U_OTHER 65534

static char *self;

static void copy_self(const char *path)
{
	char b[8192];
	int in = open(self, O_RDONLY), out;
	ssize_t r;

	if (in == -1)
		pc_unresolved("cannot open %s", self);
	out = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0700);
	if (out == -1)
		pc_unresolved("cannot create %s", path);
	while ((r = read(in, b, sizeof b)) > 0)
		if (write(out, b, (size_t)r) != r)
			pc_unresolved("short write to %s", path);
	(void)close(in);
	if (close(out) != 0)
		pc_unresolved("cannot finish %s", path);
}

/* In a child: exec the copy; 42 means it ran, otherwise the status is the errno (as 100 + errno). */
static int attempt(const char *path)
{
	pid_t kid = fork();
	int st;

	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		char *av[3];

		av[0] = (char *)path;
		av[1] = (char *)"img";
		av[2] = NULL;
		(void)execv(path, av);
		_exit(100 + (errno < 100 ? errno : 99));
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	return WIFEXITED(st) ? WEXITSTATUS(st) : -1;
}

int main(int argc, char **argv)
{
	static const struct { unsigned mode; int runs; } rows[] = {
		{ 0100, 1 }, { 0010, 1 }, { 0001, 1 }, { 0111, 1 }, { 0755, 1 },
		{ 0000, 0 }, { 0666, 0 }, { 0444, 0 }, { 0222, 0 },
	};
	size_t i;
	int r;

	if (argc >= 2 && strcmp(argv[1], "img") == 0)
		return 42;
	self = argv[0];
	pc_begin("XBD.4.7-file-access-permissions.006");
	if (geteuid() != 0)
		pc_unresolved("needs root");
	copy_self("prog");
	if (chown("prog", U_OTHER, U_OTHER) != 0)
		pc_unresolved("chown failed: %s", pc_errname(errno));
	if (chmod("prog", 0755) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	r = attempt("./prog");
	if (r != 42)
		pc_unresolved("the copy of this program cannot be run even with mode 0755 (status %d)", r);
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		if (chmod("prog", rows[i].mode) != 0)
			pc_unresolved("chmod failed: %s", pc_errname(errno));
		r = attempt("./prog");
		if (rows[i].runs)
			pc_check(r == 42, "a privileged exec of a file of mode %04o failed (status %d, errno %s)", rows[i].mode, r,
			    r >= 100 ? pc_errname(r - 100) : "-");
		else
			pc_check(r == 100 + EACCES, "a privileged exec of a file of mode %04o gave status %d, expected failure with EACCES", rows[i].mode, r);
	}
	pc_pass();
}
