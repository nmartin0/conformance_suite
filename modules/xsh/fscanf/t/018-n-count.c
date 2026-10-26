/*
 * %n stores the bytes read so far, takes an argument and is not counted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.018
 * MUTANT    fscanf.n_counted
 * MUTANT    fscanf.n_no_arg
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
	int x = -1, n = -1, r;
	char buf[8] = "";

	pc_begin("XSH.fscanf.018");
	pc_test();
	r = sscanf("ab 12  cd", "ab %d%n %s", &x, &n, buf);
	pc_check(r == 2, "sscanf() returned %d, expected 2", r);
	pc_check(x == 12, "%%d stored %d, expected 12", x);
	pc_check(n == 5, "%%n stored %d, expected 5", n);
	pc_check(strcmp(buf, "cd") == 0, "%%s stored \"%s\", expected \"cd\"", buf);
	pc_pass();
}
