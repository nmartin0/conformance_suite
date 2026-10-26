/*
 * getdelim() that fails with EINVAL sets the stream's error indicator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.007
 * MUTANT    getdelim.einval_no_indicator
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
	pc_begin("XSH.getdelim.007");
	{
		char **volatile nolp = NULL;
		FILE *f;
		size_t n = 0;
		ssize_t r;

		pc_make_file("f", "ab:");
		pc_test();
		f = open_as("f", "r");
		errno = 0;
		r = getdelim(nolp, &n, ':', f);
		NEED(r == -1 && errno == EINVAL, "getdelim() with a null lineptr did not fail with EINVAL (XSH.getdelim.006)");
		pc_check(ferror(f) != 0, "the stream's error indicator is not set after the EINVAL failure");
	}
	pc_pass();
}
