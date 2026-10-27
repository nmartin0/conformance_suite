/*
 * fgetws() on a write-only stream fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetws.007
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
	pc_begin("XSH.fgetws.007");
	{
		wchar_t ws[8];
		wchar_t *r;
		FILE *f;
		int e;

		pc_test();
		f = open_as("f", "w");
		errno = 0;
		r = fgetws(ws, 8, f);
		e = errno;
		pc_check(r == NULL, "fgetws() on a write-only stream returned %p, expected a null pointer", (void *)r);
		pc_check(e == EBADF, "fgetws() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
