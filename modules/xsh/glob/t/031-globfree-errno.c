/*
 * globfree() does not modify errno.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.031
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	glob_t g, h;
	int r;

	pc_begin("XSH.glob.031");
	pc_make_file("a", "");
	r = glob("*", 0, NULL, &g);
	NEED(r == 0, "glob() returned %d", r);
	r = glob("zz*", 0, NULL, &h);
	NEED(r == GLOB_NOMATCH, "glob() returned %d, expected GLOB_NOMATCH", r);
	pc_test();
	errno = EDOM;
	globfree(&g);
	pc_check(errno == EDOM, "errno changed to %s after globfree()", pc_errname(errno));
	errno = ERANGE;
	globfree(&h);
	pc_check(errno == ERANGE, "errno changed to %s after globfree() of a no-match result", pc_errname(errno));
	pc_pass();
}
