/*
 * fgets() on a stream whose descriptor is closed fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgets.005
 * MUTANT    fgets.errno_lost
 * MUTANT    fgets.no_error_indicator
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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
	pc_begin("XSH.fgets.005");
	{
		FILE *f;
		char s[16], *r;
		int e;

		pc_make_file("f", "0123");
		pc_test();
		f = open_as("f", "r");
		NEED(close(fileno(f)) == 0, "close() of the stream's descriptor failed: %s", pc_errname(errno));
		errno = 0;
		r = fgets(s, 16, f);
		e = errno;
		pc_check(r == NULL, "fgets() on a closed descriptor returned %p, expected an error", (void *)r);
		pc_check(e == EBADF, "fgets() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
