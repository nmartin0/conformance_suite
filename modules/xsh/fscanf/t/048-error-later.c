/*
 * An error after a conversion sets the error indicator; the count is returned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.048
 * MUTANT    fscanf.error_returns_eof
 * MUTANT    fscanf.error_clears_indicator
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	FILE *f;
	int p[2], a = -1, b = -1, r;

	pc_begin("XSH.fscanf.048");
	if (!(pipe(p) == 0))
		pc_unresolved("pipe() failed: %s", pc_errname(errno));
	pc_check(write(p[1], "12 ", 3) == 3, "write() to the pipe failed");
	if (!(fcntl(p[0], F_SETFL, O_NONBLOCK) == 0))
		pc_unresolved("fcntl(O_NONBLOCK) failed: %s", pc_errname(errno));
	f = fdopen(p[0], "r");
	pc_check(f != NULL, "fdopen() failed: %s", pc_errname(errno));
	pc_test();
	r = fscanf(f, "%d %d", &a, &b);
	pc_check(r == 1 && a == 12, "fscanf(\"%%d %%d\") returned %d and %d, expected 1 and 12", r, a);
	pc_check(ferror(f), "the error indicator is not set after the read failed with EAGAIN");
	pc_pass();
}
