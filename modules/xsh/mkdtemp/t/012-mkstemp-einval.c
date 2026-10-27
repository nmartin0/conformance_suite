/*
 * mkstemp() rejects templates that do not end in XXXXXX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.012
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
	static const char *const bad[] = { "", "fXXXXX", "fXXXXXXz" };
	char t[32];
	size_t i;
	int r;

	pc_begin("XSH.mkdtemp.012");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		strcpy(t, bad[i]);
		errno = 0;
		r = mkstemp(t);
		pc_check(r == -1, "mkstemp(\"%s\") did not fail", bad[i]);
		pc_check(errno == EINVAL, "mkstemp(\"%s\") set errno %s, expected EINVAL", bad[i], pc_errname(errno));
	}
	pc_pass();
}
