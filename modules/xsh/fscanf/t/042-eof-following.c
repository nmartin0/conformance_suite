/*
 * End of input after a conversion returns the count so far.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.042
 * MUTANT    fscanf.eof_always_eof
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
	int a = -1, b = -1, r;
	char c = 0, buf[8] = "";

	pc_begin("XSH.fscanf.042");
	pc_make_file("f", "5");
	pc_test();
	r = sscanf("5", "%d%d", &a, &b);
	pc_check(r == 1 && a == 5, "sscanf(\"5\", \"%%d%%d\") returned %d and %d, expected 1 and 5", r, a);
	r = sscanf("ab", "%s%c", buf, &c);
	pc_check(r == 1 && strcmp(buf, "ab") == 0, "sscanf(\"ab\", \"%%s%%c\") returned %d, expected 1", r);
	f = reader("f");
	a = -1;
	r = fscanf(f, "%d %d", &a, &b);
	pc_check(r == 1 && a == 5, "fscanf(\"%%d %%d\") on \"5\" returned %d and %d, expected 1 and 5", r, a);
	pc_check(feof(f) && !ferror(f), "the end-of-file indicator is %s and the error indicator %s",
	    feof(f) ? "set" : "clear", ferror(f) ? "set" : "clear");
	pc_pass();
}
