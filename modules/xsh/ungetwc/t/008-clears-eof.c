/*
 * A successful ungetwc() clears the end-of-file indicator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetwc.008
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
	pc_begin("XSH.ungetwc.008");
	{
		FILE *f;
		wint_t r, c;

		pc_make_file("f", "a");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetwc(f) == L'a', "fgetwc() did not read L'a'");
		NEED(fgetwc(f) == WEOF && feof(f), "the second read did not reach the end of the file");
		r = ungetwc(L'Z', f);
		pc_check(r == L'Z', "ungetwc(L'Z') returned %#lx, expected L'Z'", (unsigned long)r);
		pc_check(feof(f) == 0, "the end-of-file indicator is still set after a successful ungetwc()");
		c = fgetwc(f);
		pc_check(c == L'Z', "the read returned %#lx, expected L'Z'", (unsigned long)c);
		c = fgetwc(f);
		pc_check(c == WEOF, "the next read returned %#lx, expected WEOF", (unsigned long)c);
	}
	pc_pass();
}
