/*
 * Trailing white space stays unread unless the format matches it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.045
 * MUTANT    fscanf.eats_trailing_ws
 * MUTANT    fscanf.ws_noop
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
	FILE *f1, *f2;
	int x = -1, r, c;

	pc_begin("XSH.fscanf.045");
	pc_make_file("f", "5 \n x");
	pc_test();
	f1 = reader("f");
	r = fscanf(f1, "%d", &x);
	pc_check(r == 1 && x == 5, "fscanf(\"%%d\") returned %d and %d", r, x);
	c = fgetc(f1);
	pc_check(c == ' ', "after \"%%d\" the next byte read is %#x, expected the space", (unsigned)c);
	f2 = reader("f");
	r = fscanf(f2, "%d\n", &x);
	pc_check(r == 1 && x == 5, "fscanf(\"%%d\\n\") returned %d and %d", r, x);
	c = fgetc(f2);
	pc_check(c == 'x', "after \"%%d\\n\" the next byte read is %#x, expected 'x'", (unsigned)c);
	pc_pass();
}
