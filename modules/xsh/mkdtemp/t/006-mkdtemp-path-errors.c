/*
 * mkdtemp() reports a missing or non-directory leading component.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.006
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
	char a[] = "nodir/dXXXXXX", b[] = "plain/dXXXXXX";
	char *r;

	pc_begin("XSH.mkdtemp.006");
	pc_make_file("plain", "x");
	pc_test();
	errno = 0;
	r = mkdtemp(a);
	pc_check(r == NULL, "mkdtemp() under a missing directory did not fail");
	pc_check(errno == ENOENT, "missing directory: errno %s, expected ENOENT", pc_errname(errno));
	errno = 0;
	r = mkdtemp(b);
	pc_check(r == NULL, "mkdtemp() under a regular file did not fail");
	pc_check(errno == ENOTDIR, "regular file as directory: errno %s, expected ENOTDIR", pc_errname(errno));
	pc_pass();
}
