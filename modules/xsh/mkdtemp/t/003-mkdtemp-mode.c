/*
 * mkdtemp() makes the directory as mkdir(path, S_IRWXU) would.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.003
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
	char a[] = "d1XXXXXX", b[] = "d2XXXXXX";

	pc_begin("XSH.mkdtemp.003");
	pc_require_space();
	pc_test();
	umask(0);
	pc_check(mkdtemp(a) != NULL, "mkdtemp() failed: %s", pc_errname(errno));
	pc_check((pc_file_mode(a) & 0777) == 0700, "mode %04o under mask 0, expected 0700", pc_file_mode(a));
	umask(0100);
	pc_check(mkdtemp(b) != NULL, "mkdtemp() failed: %s", pc_errname(errno));
	pc_check((pc_file_mode(b) & 0777) == 0600, "mode %04o under mask 0100, expected 0600", pc_file_mode(b));
	pc_pass();
}
