/*
 * getdelim() with a null lineptr or n fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.006
 * MUTANT    getdelim.einval_returns_zero
 * MUTANT    getdelim.errno_lost
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
	pc_begin("XSH.getdelim.006");
	{
		char **volatile nolp = NULL;
		size_t *volatile non = NULL;
		FILE *f, *g;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;
		int e;

		pc_make_file("f", "ab:");
		pc_test();
		f = open_as("f", "r");
		errno = 0;
		r = getdelim(nolp, &n, ':', f);
		e = errno;
		pc_check(r == -1, "getdelim() with a null lineptr returned %ld, expected -1", (long)r);
		pc_check(e == EINVAL, "getdelim() with a null lineptr set errno %s, expected EINVAL", pc_errname(e));
		g = open_as("f", "r");
		errno = 0;
		r = getdelim(&p, non, ':', g);
		e = errno;
		pc_check(r == -1, "getdelim() with a null n returned %ld, expected -1", (long)r);
		pc_check(e == EINVAL, "getdelim() with a null n set errno %s, expected EINVAL", pc_errname(e));
		free(p);
	}
	pc_pass();
}
