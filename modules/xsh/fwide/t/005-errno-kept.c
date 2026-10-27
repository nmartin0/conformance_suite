/*
 * A successful fwide() does not change errno.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwide.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

int main(void)
{
	pc_begin("XSH.fwide.005");
	{
		FILE *a, *b, *c;
		int e1, e2, e3;

		pc_make_file("f", "abc");
		pc_test();
		a = open_as("f", "r");
		b = open_as("f", "r");
		c = open_as("f", "r");
		errno = ERANGE;
		NEED(fwide(a, 0) == 0, "fwide(a, 0) did not return 0");
		e1 = errno;
		errno = ERANGE;
		NEED(fwide(b, 1) > 0, "fwide(b, 1) did not return a positive value");
		e2 = errno;
		errno = ERANGE;
		NEED(fwide(c, -1) < 0, "fwide(c, -1) did not return a negative value");
		e3 = errno;
		pc_check(e1 == ERANGE, "fwide(a, 0) changed errno to %s", pc_errname(e1));
		pc_check(e2 == ERANGE, "fwide(b, 1) changed errno to %s", pc_errname(e2));
		pc_check(e3 == ERANGE, "fwide(c, -1) changed errno to %s", pc_errname(e3));
	}
	pc_pass();
}
