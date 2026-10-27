/*
 * fflush() discards pushed-back characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetwc.006
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
	pc_begin("XSH.ungetwc.006");
	{
		FILE *f;
		wint_t c;

		pc_make_file("f", "abc");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetwc(f) == L'a', "fgetwc() did not read L'a'");
		NEED(ungetwc(L'Z', f) == L'Z', "ungetwc() failed");
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		c = fgetwc(f);
		pc_check(c == L'b', "after fflush() the read returned %#lx, expected L'b' (the pushed-back character must be discarded)", (unsigned long)c);
	}
	pc_pass();
}
