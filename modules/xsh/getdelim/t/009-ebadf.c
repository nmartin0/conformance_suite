/*
 * getdelim() on a stream whose descriptor is closed fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.009
 * MUTANT    getdelim.errno_lost
 * MUTANT    getdelim.no_error_indicator
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
	pc_begin("XSH.getdelim.009");
	{
		FILE *f;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;
		int e;

		pc_make_file("f", "0123");
		pc_test();
		f = open_as("f", "r");
		NEED(close(fileno(f)) == 0, "close() of the stream's descriptor failed: %s", pc_errname(errno));
		errno = 0;
		r = getdelim(&p, &n, ':', f);
		e = errno;
		pc_check(r == -1, "getdelim() on a closed descriptor returned %ld, expected an error", (long)r);
		pc_check(e == EBADF, "getdelim() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
