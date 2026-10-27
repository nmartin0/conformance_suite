/*
 * A command word starting with - or + is a command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.006
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

/*
 * Creates the executable files -pcdash and +pcplus in the working directory
 * (shell scripts without an interpreter line: the shell runs a file the
 * system cannot execute itself, XCU 2.9.1.6) and puts the directory first in PATH.
 */
static void make_dash_commands(const char *body_dash, const char *body_plus)
{
	char cwd[PATH_MAX], path[PATH_MAX * 2 + 8];
	const char *old = getenv("PATH");

	pc_make_file("-pcdash", body_dash);
	pc_make_file("+pcplus", body_plus);
	if (chmod("-pcdash", 0755) != 0 || chmod("+pcplus", 0755) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed: %s", pc_errname(errno));
	(void)snprintf(path, sizeof path, "%s%s%s", cwd, old && *old ? ":" : "", old ? old : "");
	if (setenv("PATH", path, 1) != 0)
		pc_unresolved("setenv failed: %s", pc_errname(errno));
}

static void expect(const char *cmd, int code)
{
	int r = system(cmd);

	pc_check(r != -1, "system(\"%s\") failed: %s", cmd, pc_errname(errno));
	pc_check(WIFEXITED(r), "system(\"%s\") returned %#x, not a normal exit status", cmd, r);
	pc_check(WEXITSTATUS(r) == code, "system(\"%s\") gave exit status %d, expected %d", cmd, WEXITSTATUS(r), code);
}

int main(void)
{
	pc_begin("XSH.system.006");
	make_dash_commands("exit 31\n", "exit 32\n");
	pc_test();
	expect("-pcdash", 31);
	expect("+pcplus", 32);
	pc_pass();
}
