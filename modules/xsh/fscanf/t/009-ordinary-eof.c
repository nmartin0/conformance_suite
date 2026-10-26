/*
 * An ordinary directive at end of file makes the call return EOF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.009
 * MUTANT    fscanf.ordinary_eof_zero
 * MUTANT    fscanf.eof_returns_zero
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream reading path (made in setup). */
static FILE *reader(const char *path)
{
	FILE *f = fopen(path, "r");

	NEED(f != NULL, "fopen(%s) failed: %s", path, pc_errname(errno));
	return f;
}

int main(void)
{
	FILE *f;
	int x = -1, r;

	pc_begin("XSH.fscanf.009");
	pc_make_file("f", "ab");
	pc_test();
	r = sscanf("ab", "abc%d", &x);
	pc_check(r == EOF, "sscanf(\"ab\", \"abc%%d\") returned %d, expected EOF", r);
	f = reader("f");
	r = fscanf(f, "abc%d", &x);
	pc_check(r == EOF, "fscanf(\"abc%%d\") on \"ab\" returned %d, expected EOF", r);
	pc_check(feof(f), "the end-of-file indicator is not set");
	pc_check(!ferror(f), "the error indicator is set");
	pc_pass();
}
