/*
 * %c reads the field width in bytes, white space included, with no null added.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.022
 * MUTANT    fscanf.c_adds_null
 * MUTANT    fscanf.always_skip
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	char c = 0, b[5] = "ZZZZ";
	int r;

	pc_begin("XSH.fscanf.022");
	pc_test();
	r = sscanf(" xyz", "%c", &c);
	pc_check(r == 1 && c == ' ', "sscanf(\" xyz\", \"%%c\") returned %d and %#x, expected 1 and a space",
	    r, (unsigned)c);
	r = sscanf("abcdef", "%3c", b);
	pc_check(r == 1, "sscanf(\"abcdef\", \"%%3c\") returned %d, expected 1", r);
	pc_check(memcmp(b, "abcZ", 4) == 0, "the array holds %#x %#x %#x %#x, expected 'a' 'b' 'c' and 'Z' untouched",
	    (unsigned)b[0], (unsigned)b[1], (unsigned)b[2], (unsigned)b[3]);
	pc_pass();
}
