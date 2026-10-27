/*
 * The position is restored once pushed-back characters are read.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetwc.009
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
	pc_begin("XSH.ungetwc.009");
	{
		FILE *f;
		long pos;

		pc_make_file("f", "abc");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetwc(f) == L'a' && fgetwc(f) == L'b', "the reads did not return L'a' and L'b'");
		NEED(ftell(f) == 2, "ftell() is not 2 after two reads");
		NEED(ungetwc(L'b', f) == L'b', "ungetwc() failed");
		NEED(fgetwc(f) == L'b', "the pushed-back character was not read");
		pos = ftell(f);
		NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
		pc_check(pos == 2, "ftell() after reading the pushed-back character is %ld, expected 2 (the value before the push)", pos);
	}
	pc_pass();
}
