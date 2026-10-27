/*
 * With the end-of-file indicator set fgetws() returns a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetws.005
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
	pc_begin("XSH.fgetws.005");
	{
		wchar_t ws[8];
		wchar_t *r;
		FILE *f;

		pc_make_file("f", "ab");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetws(ws, 8, f) != NULL, "the first fgetws() did not read \"ab\"");
		NEED(fgetws(ws, 8, f) == NULL && feof(f), "the second fgetws() did not reach the end of the file");
		append_other("f", "c");
		r = fgetws(ws, 8, f);
		pc_check(r == NULL, "with the end-of-file indicator set, fgetws() returned %p after the file grew, expected a null pointer", (void *)r);
		pc_check(feof(f) != 0, "the end-of-file indicator is no longer set");
	}
	pc_pass();
}
