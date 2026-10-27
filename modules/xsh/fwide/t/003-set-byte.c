/*
 * fwide() with a negative mode orients a new stream as byte, for good.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwide.003
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
	pc_begin("XSH.fwide.003");
	{
		FILE *f;
		int o;

		pc_make_file("f", "abc");
		pc_test();
		f = open_as("f", "r");
		o = fwide(f, -5);
		pc_check(o < 0, "fwide(f, -5) on a new stream returned %d, expected a negative value", o);
		o = fwide(f, 0);
		pc_check(o < 0, "fwide(f, 0) afterwards returned %d, expected a negative value", o);
		o = fwide(f, 1);
		pc_check(o < 0, "fwide(f, 1) on a byte-oriented stream returned %d, expected the orientation to stay negative", o);
	}
	pc_pass();
}
