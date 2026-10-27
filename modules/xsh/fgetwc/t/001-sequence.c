/*
 * fgetwc() returns successive characters and advances the position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetwc.001
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
	pc_begin("XSH.fgetwc.001");
	{
		static const wint_t want[3] = { L'a', L'Z', L'0' };
		FILE *f;
		wint_t c;
		long pos;
		int i;

		pc_make_file("f", "aZ0");
		pc_test();
		f = open_as("f", "r");
		for (i = 0; i < 3; i++) {
			c = fgetwc(f);
			pc_check(c == want[i], "call %d of fgetwc() returned %#lx, expected %#lx", i + 1, (unsigned long)c, (unsigned long)want[i]);
			pos = ftell(f);
			NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
			pc_check(pos == i + 1, "ftell() after call %d is %ld, expected %d", i + 1, pos, i + 1);
		}
	}
	pc_pass();
}
