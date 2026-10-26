/*
 * A differing byte ends an ordinary directive and stays unread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.008
 * MUTANT    fscanf.mismatch_consumed
 * MUTANT    fscanf.matching_returns_eof
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
	int x = -1, r, c1, c2;

	pc_begin("XSH.fscanf.008");
	pc_make_file("f", "abXd5");
	pc_test();
	f = reader("f");
	r = fscanf(f, "abc%d", &x);
	pc_check(r == 0, "fscanf(\"abc%%d\") on \"abXd5\" returned %d, expected 0", r);
	c1 = fgetc(f);
	c2 = fgetc(f);
	pc_check(c1 == 'X' && c2 == 'd', "the next reads returned %#x and %#x, expected 'X' and 'd'",
	    (unsigned)c1, (unsigned)c2);
	pc_pass();
}
