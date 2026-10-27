/*
 * fwscanf() on a stream whose descriptor is closed fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwscanf.015
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
	pc_begin("XSH.fwscanf.015");
	{
		FILE *f;
		int i, r, e;

		pc_make_file("f", "12");
		pc_test();
		f = open_as("f", "r");
		NEED(close(fileno(f)) == 0, "close() of the stream's descriptor failed: %s", pc_errname(errno));
		errno = 0;
		r = fwscanf(f, L"%d", &i);
		e = errno;
		pc_check(r == EOF, "fwscanf() on a closed descriptor returned %d, expected EOF", r);
		pc_check(e == EBADF, "fwscanf() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
