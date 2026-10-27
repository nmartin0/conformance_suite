/*
 * The first wide or byte input/output function orients a stream.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwide.004
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
	pc_begin("XSH.fwide.004");
	{
		FILE *a, *b, *c, *d;
		int o;

		pc_make_file("f", "abc");
		pc_test();
		a = open_as("f", "r");
		b = open_as("g", "w");
		c = open_as("f", "r");
		d = open_as("h", "w");
		NEED(fgetwc(a) == L'a', "fgetwc() did not read L'a'");
		NEED(fputwc(L'x', b) == L'x', "fputwc() did not write L'x'");
		NEED(fgetc(c) == 'a', "fgetc() did not read 'a'");
		NEED(fputc('x', d) == 'x', "fputc() did not write 'x'");
		o = fwide(a, 0);
		pc_check(o > 0, "after fgetwc() fwide() returned %d, expected a positive value", o);
		o = fwide(b, 0);
		pc_check(o > 0, "after fputwc() fwide() returned %d, expected a positive value", o);
		o = fwide(c, 0);
		pc_check(o < 0, "after fgetc() fwide() returned %d, expected a negative value", o);
		o = fwide(d, 0);
		pc_check(o < 0, "after fputc() fwide() returned %d, expected a negative value", o);
	}
	pc_pass();
}
