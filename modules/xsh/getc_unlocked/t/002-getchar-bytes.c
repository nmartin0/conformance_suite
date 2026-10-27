/*
 * getchar_unlocked() reads standard input in order and shares its state with getc_unlocked(stdin).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getc_unlocked.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* Opens path with flags and makes it descriptor target (setup). */
static void fd_onto(const char *path, int flags, int target)
{
	int fd = open(path, flags, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (fd != target) {
		if (!(dup2(fd, target) == target))
			pc_unresolved("dup2 to %d failed: %s", target, pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
	}
}

int main(void)
{
	pc_begin("XSH.getc_unlocked.002");
	{
		static const char want[] = "abcd";
		int i, c;

		pc_make_file("f", "abcd");
		fd_onto("f", O_RDONLY, 0);
		pc_test();
		flockfile(stdin);
		for (i = 0; i < 4; i++) {
			c = (i % 2 == 0) ? getchar_unlocked() : getc_unlocked(stdin);
			pc_check(c == want[i], "read number %d (%s) returned %d, expected %d", i + 1, i % 2 == 0 ? "getchar_unlocked()" : "getc_unlocked(stdin)", c, want[i]);
		}
		c = getchar_unlocked();
		pc_check(c == EOF, "getchar_unlocked() at the end of the file returned %d, expected EOF", c);
		pc_check(feof(stdin) != 0, "the end-of-file indicator of stdin is not set");
		c = getc_unlocked(stdin);
		pc_check(c == EOF, "getc_unlocked(stdin) after the end was reached returned %d, expected EOF", c);
		funlockfile(stdin);
	}
	pc_pass();
}
