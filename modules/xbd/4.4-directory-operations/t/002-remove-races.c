/*
 * Processes that all remove (unlink, rmdir) or rename away the same name at
 * once: exactly one succeeds and every other one fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.4-directory-operations.002
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

static const char *kinds[] = { "unlink", "rmdir", "rename (file)", "rename (directory)" };
static char name[64];

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

/* In child i: 0 on success, 1 for ENOENT, 2 for anything else. */
static int attempt(int kind, int i)
{
	int r;
	char to[96];

	switch (kind) {
	case 0:
		r = unlink(name);
		break;
	case 1:
		r = rmdir(name);
		break;
	default:
		(void)snprintf(to, sizeof to, "%s.to%d", name, i);
		r = rename(name, to);
		break;
	}
	if (r == 0)
		return 0;
	return errno == ENOENT ? 1 : 2;
}

int main(void)
{
	int kind, round, i, go[2], st, wins, enoent, other, fd;
	pid_t kid[NKIDS];
	char c, to[96];
	struct stat sb;

	pc_begin("XBD.4.4-directory-operations.002");
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(120);
	for (kind = 0; kind < 4; kind++) {
		for (round = 0; round < ROUNDS; round++) {
			(void)snprintf(name, sizeof name, "n.%d.%d", kind, round);
			if (kind == 1 || kind == 3) {
				if (mkdir(name, 0700) != 0)
					pc_unresolved("mkdir failed: %s", pc_errname(errno));
			} else {
				fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
				if (fd < 0)
					pc_unresolved("creating %s failed: %s", name, pc_errname(errno));
				(void)close(fd);
			}
			if (pipe(go) != 0)
				pc_unresolved("pipe failed: %s", pc_errname(errno));
			for (i = 0; i < NKIDS; i++) {
				kid[i] = fork();
				if (kid[i] == -1)
					pc_unresolved("fork failed: %s", pc_errname(errno));
				if (kid[i] == 0) {
					(void)close(go[1]);
					(void)read(go[0], &c, 1);
					_exit(attempt(kind, i));
				}
			}
			(void)close(go[0]);
			(void)close(go[1]);
			wins = enoent = other = 0;
			for (i = 0; i < NKIDS; i++) {
				if (waitpid(kid[i], &st, 0) != kid[i])
					pc_unresolved("waitpid failed: %s", pc_errname(errno));
				if (!WIFEXITED(st))
					pc_unresolved("a child ended abnormally (status %#x)", (unsigned)st);
				switch (WEXITSTATUS(st)) {
				case 0: wins++; break;
				case 1: enoent++; break;
				default: other++; break;
				}
			}
			pc_check(wins == 1 && enoent == NKIDS - 1 && other == 0,
			    "%s of one name by %d processes at once (round %d): %d succeeded, %d failed with ENOENT, %d failed otherwise",
			    kinds[kind], NKIDS, round, wins, enoent, other);
			pc_check(lstat(name, &sb) != 0 && errno == ENOENT, "the name %s still exists after a successful %s", name, kinds[kind]);
			if (kind >= 2) {
				int found = 0;

				for (i = 0; i < NKIDS; i++) {
					(void)snprintf(to, sizeof to, "%s.to%d", name, i);
					if (lstat(to, &sb) == 0) {
						found++;
						if (kind == 3)
							(void)rmdir(to);
						else
							(void)unlink(to);
					}
				}
				pc_check(found == 1, "%d of the rename targets exist after the renames, expected exactly 1", found);
			}
		}
	}
	pc_pass();
}
