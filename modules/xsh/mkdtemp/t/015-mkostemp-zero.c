/*
 * mkostemp() with no flags behaves as mkstemp().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.015
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

static int portable(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
}

/* The last six bytes of t are portable filename characters. */
static int tail_portable(const char *t)
{
	size_t n = strlen(t), i;

	if (n < 6)
		return 0;
	for (i = n - 6; i < n; i++)
		if (!portable(t[i]))
			return 0;
	return 1;
}


int main(void)
{
	char t[] = "sub/fXXXXXX", bad[16];
	struct stat st;
	int fd, fl;

	pc_begin("XSH.mkdtemp.015");
	pc_make_dir("sub");
	pc_require_space();
	pc_test();
	umask(0);
	fd = mkostemp(t, 0);
	pc_expect_ok(fd, "mkostemp()");
	pc_check(strncmp(t, "sub/f", 5) == 0 && tail_portable(t), "the name was not made as for mkstemp(): %s", t);
	pc_check(fstat(fd, &st) == 0, "fstat failed");
	pc_check(S_ISREG(st.st_mode) && st.st_size == 0, "not a new regular file of length 0");
	pc_check((st.st_mode & 0777) == 0600, "mode %04o, expected 0600", (unsigned)(st.st_mode & 07777));
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1 && (fl & O_ACCMODE) == O_RDWR, "access mode is not O_RDWR");
	fl = fcntl(fd, F_GETFD);
	pc_check(fl != -1 && (fl & FD_CLOEXEC) == 0, "FD_CLOEXEC is set");
	strcpy(bad, "fXXXXX");
	errno = 0;
	pc_check(mkostemp(bad, 0) == -1 && errno == EINVAL, "mkostemp() of a template without six X did not fail with EINVAL");
	pc_pass();
}
