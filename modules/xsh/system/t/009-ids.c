/*
 * The shell has its own process ID and the caller is its parent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.009
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* The file's contents (up to 255 bytes) as a string; "" if it cannot be read. */
static const char *slurp_file(const char *path)
{
	static char b[256];
	int f = open(path, O_RDONLY);
	ssize_t n;

	b[0] = '\0';
	if (f < 0)
		return b;
	n = read(f, b, sizeof b - 1);
	(void)close(f);
	if (n > 0)
		b[n] = '\0';
	else
		b[0] = '\0';
	return b;
}

int main(void)
{
	long shell = -1, parent = -1;
	int r;

	pc_begin("XSH.system.009");
	pc_test();
	r = system("echo $$ $PPID > ids");
	pc_check(r != -1 && WIFEXITED(r) && WEXITSTATUS(r) == 0, "system() gave %d", r);
	pc_check(sscanf(slurp_file("ids"), "%ld %ld", &shell, &parent) == 2, "the command wrote \"%s\"", slurp_file("ids"));
	pc_check(shell > 0 && shell != (long)getpid(), "the shell's process ID %ld is not different from the caller's %ld", shell, (long)getpid());
	pc_check(parent == (long)getpid(), "the shell's parent process ID is %ld, the caller's is %ld", parent, (long)getpid());
	pc_pass();
}
