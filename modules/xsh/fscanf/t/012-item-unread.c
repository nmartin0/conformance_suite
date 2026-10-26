/*
 * The byte after an input item, and a conflicting byte, stay unread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.012
 * MUTANT    fscanf.item_next_lost
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
	FILE *fa, *fb, *fc;
	int x = -1, r, c;

	pc_begin("XSH.fscanf.012");
	pc_make_file("a", "123abc");
	pc_make_file("b", "12345");
	pc_make_file("c", "abc");
	pc_test();
	fa = reader("a");
	r = fscanf(fa, "%d", &x);
	pc_check(r == 1 && x == 123, "fscanf(\"%%d\") on \"123abc\" returned %d and %d", r, x);
	c = fgetc(fa);
	pc_check(c == 'a', "after it the next byte read is %#x, expected 'a'", (unsigned)c);
	fb = reader("b");
	r = fscanf(fb, "%3d", &x);
	pc_check(r == 1 && x == 123, "fscanf(\"%%3d\") on \"12345\" returned %d and %d", r, x);
	c = fgetc(fb);
	pc_check(c == '4', "after it the next byte read is %#x, expected '4'", (unsigned)c);
	fc = reader("c");
	r = fscanf(fc, "%d", &x);
	pc_check(r == 0, "fscanf(\"%%d\") on \"abc\" returned %d, expected 0", r);
	c = fgetc(fc);
	pc_check(c == 'a', "after it the next byte read is %#x, expected 'a'", (unsigned)c);
	pc_pass();
}
