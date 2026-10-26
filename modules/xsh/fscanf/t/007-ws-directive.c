/*
 * A white-space directive skips white space and never fails.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.007
 * MUTANT    fscanf.ws_eats_one
 * MUTANT    fscanf.ws_must_match
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
	char c = 0;
	int n = -1, r, k;

	pc_begin("XSH.fscanf.007");
	pc_make_file("f", "a \t\n\v\f b!");
	pc_test();
	f = reader("f");
	r = fscanf(f, "a %c", &c);
	pc_check(r == 1 && c == 'b', "fscanf(\"a %%c\") returned %d and %#x, expected 1 and 'b'", r, (unsigned)c);
	k = fgetc(f);
	pc_check(k == '!', "the next fgetc() returned %#x, expected '!'", (unsigned)k);
	r = sscanf("ab", "a b%n", &n);
	pc_check(r == 0, "sscanf(\"ab\", \"a b%%n\") returned %d, expected 0", r);
	pc_check(n == 2, "%%n stored %d, expected 2 (the directive matched no white space)", n);
	pc_pass();
}
