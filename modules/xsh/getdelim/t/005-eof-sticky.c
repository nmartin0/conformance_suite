/*
 * getdelim() with the end-of-file indicator set returns -1 even when data follows.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.005
 * MUTANT    getdelim.eof_not_sticky
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fopen.read_all
 * VARIANT   getdelim.n_exact
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

/* Appends s to path through a new open file description. */
static void append_other(const char *path, const char *s)
{
	int fd = open(path, O_WRONLY | O_APPEND);
	size_t n = strlen(s);

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	NEED(write(fd, s, n) == (ssize_t)n, "write(%s) failed: %s", path, pc_errname(errno));
	NEED(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.getdelim.005");
	{
		FILE *f;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;

		pc_make_file("f", "ab");
		pc_test();
		f = open_as("f", "r");
		NEED(getdelim(&p, &n, ':', f) == 2, "the first getdelim() of \"ab\" did not return 2");
		NEED(getdelim(&p, &n, ':', f) == -1 && feof(f), "the second getdelim() did not report end of file");
		append_other("f", "c:");
		r = getdelim(&p, &n, ':', f);
		pc_check(r == -1, "with the end-of-file indicator set, getdelim() returned %ld after the file grew, expected -1", (long)r);
		pc_check(feof(f) != 0, "the end-of-file indicator is no longer set");
		free(p);
	}
	pc_pass();
}
