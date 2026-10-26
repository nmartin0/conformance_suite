/*
 * A scanset conversion needs at least one byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.021
 * MUTANT    fscanf.scanset_empty_ok
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
	char buf[8] = "";
	int r, c;

	pc_begin("XSH.fscanf.021");
	pc_make_file("f", "xyz");
	pc_test();
	r = sscanf("xyz", "%[abc]", buf);
	pc_check(r == 0, "sscanf(\"xyz\", \"%%[abc]\") returned %d, expected 0", r);
	f = reader("f");
	r = fscanf(f, "%[abc]", buf);
	pc_check(r == 0, "fscanf(\"%%[abc]\") on \"xyz\" returned %d, expected 0", r);
	c = fgetc(f);
	pc_check(c == 'x', "the next fgetc() returned %#x, expected 'x'", (unsigned)c);
	pc_pass();
}
