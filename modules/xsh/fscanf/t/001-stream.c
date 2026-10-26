/*
 * fscanf() reads from the stream it is given.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.001
 * MUTANT    fscanf.reads_stdin
 * MUTANT    fscanf.item_next_lost
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
	int a = -1, b = -1, r, c;

	pc_begin("XSH.fscanf.001");
	pc_make_file("f", "12 34 rest");
	pc_test();
	f = reader("f");
	r = fscanf(f, "%d%d", &a, &b);
	pc_check(r == 2, "fscanf() returned %d, expected 2", r);
	pc_check(a == 12 && b == 34, "stored %d and %d, expected 12 and 34", a, b);
	c = fgetc(f);
	pc_check(c == ' ', "the next fgetc() returned %#x, expected the space after 34", (unsigned)c);
	pc_pass();
}
