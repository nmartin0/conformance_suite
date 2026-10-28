/*
 * While one process keeps renaming new files over a name, other processes
 * opening that name always find a file, whole, and never an older one than
 * they saw before.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.4-directory-operations.004
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define NREAD 3
#define GENS 3000

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

static void put_gen(const char *path, int g)
{
	char b[32];
	int fd, n = snprintf(b, sizeof b, "GEN%08d\n", g);

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0 || write(fd, b, (size_t)n) != n || close(fd) != 0)
		_exit(10);
}

/* Reader: 0 ok, 1 name missing, 2 bad content, 3 generation went back, 4 other error. */
static int reader(void)
{
	int last = -1, g, fd, n, done = 0, loops = 0;
	char b[32];
	struct stat sb;

	(void)signal(SIGALRM, SIG_DFL);
	(void)alarm(100);
	while (!done) {
		fd = open("T", O_RDONLY);
		if (fd < 0)
			return errno == ENOENT ? 1 : 4;
		n = (int)read(fd, b, sizeof b);
		(void)close(fd);
		if (n != 12 || strncmp(b, "GEN", 3) != 0 || b[11] != '\n')
			return 2;
		b[11] = '\0';
		g = atoi(b + 3);
		if (g < last)
			return 3;
		last = g;
		if ((++loops & 31) == 0 && stat("done", &sb) == 0)
			done = 1;
	}
	return 0;
}

int main(void)
{
	pid_t kid[NREAD];
	int i, g, st, seen[NREAD];
	int fd;

	pc_begin("XBD.4.4-directory-operations.004");
	(void)signal(SIGALRM, on_alarm);
	put_gen("T", 0);
	pc_test();
	(void)alarm(120);
	for (i = 0; i < NREAD; i++) {
		kid[i] = fork();
		if (kid[i] == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid[i] == 0)
			_exit(reader());
	}
	for (g = 1; g <= GENS; g++) {
		put_gen("N", g);
		if (rename("N", "T") != 0) {
			pc_unresolved("rename failed: %s", pc_errname(errno));
		}
	}
	fd = open("done", O_WRONLY | O_CREAT, 0600);
	if (fd >= 0)
		(void)close(fd);
	for (i = 0; i < NREAD; i++) {
		if (waitpid(kid[i], &st, 0) != kid[i])
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
		seen[i] = WIFEXITED(st) ? WEXITSTATUS(st) : 99;
	}
	for (i = 0; i < NREAD; i++) {
		pc_check(seen[i] != 1, "reader %d did not find the name while it was being replaced by rename()", i);
		pc_check(seen[i] != 2, "reader %d read a file that was not one of the complete files", i);
		pc_check(seen[i] != 3, "reader %d found an older file after a newer one", i);
		pc_check(seen[i] == 0, "reader %d ended with code %d", i, seen[i]);
	}
	pc_pass();
}
