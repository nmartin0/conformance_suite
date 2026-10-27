/*
 * Repeated mkstemp() calls give different new files and leave existing ones alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.011
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
	static const char *const decoy[] = { "f000000", "f000001", "fAAAAAA", "faaaaaa", "fXXXXXX", "f111111" };
	static char names[N][16];
	size_t i, j;
	char t[16];
	int fd;

	pc_begin("XSH.mkdtemp.011");
	for (i = 0; i < sizeof decoy / sizeof decoy[0]; i++)
		pc_make_file(decoy[i], "decoy");
	pc_require_space();
	pc_test();
	for (i = 0; i < N; i++) {
		strcpy(t, "fXXXXXX");
		fd = mkstemp(t);
		pc_expect_ok(fd, "mkstemp()");
		close(fd);
		for (j = 0; j < sizeof decoy / sizeof decoy[0]; j++)
			pc_check(strcmp(t, decoy[j]) != 0, "call %zu returned the name of a file that existed: %s", i, t);
		for (j = 0; j < i; j++)
			pc_check(strcmp(t, names[j]) != 0, "calls %zu and %zu returned the same name %s", j, i, t);
		strcpy(names[i], t);
	}
	for (j = 0; j < sizeof decoy / sizeof decoy[0]; j++)
		pc_check(pc_file_equals(decoy[j], "decoy"), "the existing file %s was changed", decoy[j]);
	pc_pass();
}
