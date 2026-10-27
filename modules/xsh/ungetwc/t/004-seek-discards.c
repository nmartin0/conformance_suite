/*
 * fseek() and rewind() discard pushed-back characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetwc.004
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
	pc_begin("XSH.ungetwc.004");
	{
		FILE *f;
		wint_t c;

		pc_make_file("f", "abc");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetwc(f) == L'a', "fgetwc() did not read L'a'");
		NEED(ungetwc(L'Z', f) == L'Z', "ungetwc() failed");
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek() failed: %s", pc_errname(errno));
		c = fgetwc(f);
		pc_check(c == L'a', "after fseek() the read returned %#lx, expected L'a' (the pushed-back character must be discarded)", (unsigned long)c);
		NEED(ungetwc(L'Z', f) == L'Z', "the second ungetwc() failed");
		rewind(f);
		c = fgetwc(f);
		pc_check(c == L'a', "after rewind() the read returned %#lx, expected L'a'", (unsigned long)c);
	}
	pc_pass();
}
