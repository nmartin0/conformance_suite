/*
 * A field width bounds the input item.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.016
 * MUTANT    fscanf.width_ignored
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
	int a = -1, b = -1, r;
	char s1[8] = "", s2[8] = "", c2[2] = { 0, 0 }, c = 0;

	pc_begin("XSH.fscanf.016");
	pc_test();
	r = sscanf("12345", "%2d%d", &a, &b);
	pc_check(r == 2 && a == 12 && b == 345, "sscanf(\"12345\", \"%%2d%%d\") returned %d, %d, %d", r, a, b);
	r = sscanf("abcdef", "%3s%s", s1, s2);
	pc_check(r == 2 && strcmp(s1, "abc") == 0 && strcmp(s2, "def") == 0,
	    "sscanf(\"abcdef\", \"%%3s%%s\") returned %d, \"%s\", \"%s\"", r, s1, s2);
	r = sscanf("abcdef", "%2c%c", c2, &c);
	pc_check(r == 2 && c2[0] == 'a' && c2[1] == 'b' && c == 'c',
	    "sscanf(\"abcdef\", \"%%2c%%c\") returned %d, %#x %#x, %#x", r,
	    (unsigned)c2[0], (unsigned)c2[1], (unsigned)c);
	pc_pass();
}
