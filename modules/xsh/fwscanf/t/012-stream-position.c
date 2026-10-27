/*
 * Input after the converted item, including trailing white space, stays unread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwscanf.012
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
	pc_begin("XSH.fwscanf.012");
	{
		FILE *f;
		int i, r;
		wint_t c;

		pc_make_file("f", "10 20\n");
		pc_test();
		f = open_as("f", "r");
		i = 0;
		r = fwscanf(f, L"%d", &i);
		pc_check(r == 1 && i == 10, "fwscanf() returned %d with i = %d, expected 1 and 10", r, i);
		c = fgetwc(f);
		pc_check(c == L' ', "the character after the item is %#lx, expected a space left unread", (unsigned long)c);
		i = 0;
		r = fwscanf(f, L"%d", &i);
		pc_check(r == 1 && i == 20, "the second fwscanf() returned %d with i = %d, expected 1 and 20", r, i);
		c = fgetwc(f);
		pc_check(c == L'\n', "the character after 20 is %#lx, expected the newline left unread", (unsigned long)c);
	}
	pc_pass();
}
