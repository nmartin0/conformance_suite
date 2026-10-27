/*
 * mkdtemp() returns its argument and makes a directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.001
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
	char t[] = "dXXXXXX";
	struct stat st;
	char *r;

	pc_begin("XSH.mkdtemp.001");
	pc_require_space();
	pc_test();
	r = mkdtemp(t);
	pc_check(r != NULL, "mkdtemp() failed: %s", pc_errname(errno));
	pc_check(r == t, "mkdtemp() did not return its argument");
	pc_check(lstat(t, &st) == 0, "nothing exists at the name %s", t);
	pc_check(S_ISDIR(st.st_mode), "%s is not a directory", t);
	pc_pass();
}
