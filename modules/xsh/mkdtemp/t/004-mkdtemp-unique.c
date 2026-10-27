/*
 * Repeated mkdtemp() calls give different new directories.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.004
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


#define N 100

int main(void)
{
	static const char *const decoy[] = { "d000000", "d000001", "dAAAAAA", "daaaaaa", "dXXXXXX", "d111111" };
	static char names[N][16];
	struct stat st;
	size_t i, j;
	char t[16];

	pc_begin("XSH.mkdtemp.004");
	for (i = 0; i < sizeof decoy / sizeof decoy[0]; i++)
		pc_make_dir(decoy[i]);
	pc_require_space();
	pc_test();
	for (i = 0; i < N; i++) {
		strcpy(t, "dXXXXXX");
		pc_check(mkdtemp(t) != NULL, "call %zu: mkdtemp() failed: %s", i, pc_errname(errno));
		pc_check(lstat(t, &st) == 0 && S_ISDIR(st.st_mode), "call %zu: no directory at %s", i, t);
		for (j = 0; j < sizeof decoy / sizeof decoy[0]; j++)
			pc_check(strcmp(t, decoy[j]) != 0, "call %zu returned the name of a directory that existed: %s", i, t);
		for (j = 0; j < i; j++)
			pc_check(strcmp(t, names[j]) != 0, "calls %zu and %zu returned the same name %s", j, i, t);
		strcpy(names[i], t);
	}
	pc_pass();
}
