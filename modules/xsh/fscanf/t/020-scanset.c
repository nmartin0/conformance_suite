/*
 * The scanset rules for ']' and '^'.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.020
 * MUTANT    fscanf.scanset_no_leading_bracket
 * MUTANT    fscanf.scanset_no_negate
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
	char buf[16];
	int n = -1, r;

	pc_begin("XSH.fscanf.020");
	pc_test();
	buf[0] = '\0';
	r = sscanf("]a]b", "%[]a]", buf);
	pc_check(r == 1 && strcmp(buf, "]a]") == 0, "\"%%[]a]\" on \"]a]b\" returned %d and \"%s\"", r, buf);
	buf[0] = '\0';
	r = sscanf("abc]d", "%[^]d]", buf);
	pc_check(r == 1 && strcmp(buf, "abc") == 0, "\"%%[^]d]\" on \"abc]d\" returned %d and \"%s\"", r, buf);
	buf[0] = '\0';
	r = sscanf("abc]", "%[abc]%n", buf, &n);
	pc_check(r == 1 && strcmp(buf, "abc") == 0 && n == 3,
	    "\"%%[abc]%%n\" on \"abc]\" returned %d, \"%s\" and %d", r, buf, n);
	buf[0] = '\0';
	r = sscanf("x y,z", "%[^,]", buf);
	pc_check(r == 1 && strcmp(buf, "x y") == 0, "\"%%[^,]\" on \"x y,z\" returned %d and \"%s\"", r, buf);
	pc_pass();
}
