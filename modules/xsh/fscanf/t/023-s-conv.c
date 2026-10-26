/*
 * %s reads a run of non-white-space bytes and adds a null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.023
 * MUTANT    fscanf.s_no_null
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
	char b[8];
	int n = -1, r, c;

	pc_begin("XSH.fscanf.023");
	pc_make_file("f", "ab cd");
	pc_test();
	memset(b, 'Z', sizeof b - 1);
	b[sizeof b - 1] = '\0';
	r = sscanf("  ab\tcd", "%s%n", b, &n);
	pc_check(r == 1 && strcmp(b, "ab") == 0, "sscanf(\"  ab\\tcd\", \"%%s%%n\") returned %d and \"%s\"", r, b);
	pc_check(n == 4, "%%n stored %d, expected 4", n);
	f = reader("f");
	r = fscanf(f, "%s", b);
	pc_check(r == 1 && strcmp(b, "ab") == 0, "fscanf(\"%%s\") on \"ab cd\" returned %d and \"%s\"", r, b);
	c = fgetc(f);
	pc_check(c == ' ', "the next fgetc() returned %#x, expected the space", (unsigned)c);
	pc_pass();
}
