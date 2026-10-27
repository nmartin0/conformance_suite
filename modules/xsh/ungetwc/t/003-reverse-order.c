/*
 * Pushed-back characters come back in reverse order; one is guaranteed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetwc.003
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
	pc_begin("XSH.ungetwc.003");
	{
		FILE *f;
		wint_t r1, r2, c;

		pc_make_file("f", "abc");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetwc(f) == L'a', "fgetwc() did not read L'a'");
		r1 = ungetwc(L'1', f);
		pc_check(r1 == L'1', "the first ungetwc() returned %#lx, expected L'1' (one character of push-back is guaranteed)", (unsigned long)r1);
		r2 = ungetwc(L'2', f);
		if (r2 == WEOF) {
			c = fgetwc(f);
			pc_check(c == L'1', "after the failed second push the read returned %#lx, expected L'1'", (unsigned long)c);
		} else {
			pc_check(r2 == L'2', "the second ungetwc() returned %#lx, expected L'2' or WEOF", (unsigned long)r2);
			c = fgetwc(f);
			pc_check(c == L'2', "the first read returned %#lx, expected L'2' (last pushed, first read)", (unsigned long)c);
			c = fgetwc(f);
			pc_check(c == L'1', "the second read returned %#lx, expected L'1'", (unsigned long)c);
		}
	}
	pc_pass();
}
