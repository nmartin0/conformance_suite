/*
 * A sign alone is a matching failure for %d.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.013
 * MUTANT    fscanf.lone_sign
 * MUTANT    fscanf.conflict_lost
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
	int x = -1, r, c;

	pc_begin("XSH.fscanf.013");
	pc_make_file("f", "-x");
	pc_test();
	r = sscanf("-x", "%d", &x);
	pc_check(r == 0, "sscanf(\"-x\", \"%%d\") returned %d, expected 0", r);
	r = sscanf("+ 1", "%d", &x);
	pc_check(r == 0, "sscanf(\"+ 1\", \"%%d\") returned %d, expected 0", r);
	f = reader("f");
	r = fscanf(f, "%d", &x);
	pc_check(r == 0, "fscanf(\"%%d\") on \"-x\" returned %d, expected 0", r);
	c = fgetc(f);
	pc_check(c == 'x', "the next fgetc() returned %#x, expected 'x' (the byte after the item \"-\")",
	    (unsigned)c);
	pc_pass();
}
