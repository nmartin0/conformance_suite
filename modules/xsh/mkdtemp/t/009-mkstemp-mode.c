/*
 * mkstemp() makes the file as open(..., S_IRUSR|S_IWUSR) would.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.009
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)


int main(void)
{
	static const struct { mode_t mask; unsigned want; } c[] = { { 0, 0600 }, { 077, 0600 }, { 0200, 0400 } };
	struct stat st;
	char t[16];
	size_t i;
	int fd;

	pc_begin("XSH.mkdtemp.009");
	pc_require_space();
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		strcpy(t, "fXXXXXX");
		umask(c[i].mask);
		fd = mkstemp(t);
		pc_expect_ok(fd, "mkstemp()");
		pc_check(fstat(fd, &st) == 0, "fstat failed");
		pc_check((st.st_mode & 0777) == c[i].want, "mode %04o under mask %03o, expected %04o",
			 (unsigned)(st.st_mode & 07777), (unsigned)c[i].mask, c[i].want);
		close(fd);
	}
	pc_pass();
}
