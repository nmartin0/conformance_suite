/*
 * Processes that all create the same name at once (open with O_EXCL,
 * mkdir, link, symlink, mkfifo): exactly one succeeds and every other one
 * fails with EEXIST.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.4-directory-operations.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define NKIDS 6
#define ROUNDS 40

static const char *kinds[] = { "open(O_CREAT|O_EXCL)", "mkdir", "link", "symlink", "mkfifo" };
static char name[64];

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

/* In a child: the operation under test; 0 on success, 1 for EEXIST, 2 for anything else. */
static int attempt(int kind)
{
	int r, fd;

	switch (kind) {
	case 0:
		fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
		r = fd < 0 ? -1 : 0;
		if (fd >= 0)
			(void)close(fd);
		break;
	case 1:
		r = mkdir(name, 0700);
		break;
	case 2:
		r = link("src", name);
		break;
	case 3:
		r = symlink("somewhere", name);
		break;
	default:
		r = mkfifo(name, 0600);
		break;
	}
	if (r == 0)
		return 0;
	return errno == EEXIST ? 1 : 2;
}

int main(void)
{
	int kind, round, i, go[2], st, wins, eex, other;
	pid_t kid[NKIDS];
	char c;
	struct stat sb;

	pc_begin("XBD.4.4-directory-operations.001");
	(void)signal(SIGALRM, on_alarm);
	pc_make_file("src", "x");
	pc_test();
	(void)alarm(120);
	for (kind = 0; kind < 5; kind++) {
		for (round = 0; round < ROUNDS; round++) {
			(void)snprintf(name, sizeof name, "n.%d.%d", kind, round);
			if (pipe(go) != 0)
				pc_unresolved("pipe failed: %s", pc_errname(errno));
			for (i = 0; i < NKIDS; i++) {
				kid[i] = fork();
				if (kid[i] == -1)
					pc_unresolved("fork failed: %s", pc_errname(errno));
				if (kid[i] == 0) {
					(void)close(go[1]);
					(void)read(go[0], &c, 1);	/* returns at end of file: all start together */
					_exit(attempt(kind));
				}
			}
			(void)close(go[0]);
			(void)close(go[1]);
			wins = eex = other = 0;
			for (i = 0; i < NKIDS; i++) {
				if (waitpid(kid[i], &st, 0) != kid[i])
					pc_unresolved("waitpid failed: %s", pc_errname(errno));
				if (!WIFEXITED(st))
					pc_unresolved("a child ended abnormally (status %#x)", (unsigned)st);
				switch (WEXITSTATUS(st)) {
				case 0: wins++; break;
				case 1: eex++; break;
				default: other++; break;
				}
			}
			pc_check(wins == 1 && eex == NKIDS - 1 && other == 0,
			    "%s of one name by %d processes at once (round %d): %d succeeded, %d failed with EEXIST, %d failed otherwise",
			    kinds[kind], NKIDS, round, wins, eex, other);
			pc_check(lstat(name, &sb) == 0, "the name %s does not exist after a successful %s", name, kinds[kind]);
			if (kind == 1)
				(void)rmdir(name);
			else
				(void)unlink(name);
		}
	}
	pc_pass();
}
