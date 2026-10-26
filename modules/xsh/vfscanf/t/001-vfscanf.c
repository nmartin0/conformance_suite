/*
 * vfscanf() is fscanf() with a va_list.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfscanf.001
 * MUTANT    vfscanf.reads_stdin
 * MUTANT    vfscanf.item_next_lost
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static int call(FILE *f, const char *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfscanf(f, fmt, ap);
	va_end(ap);
	return r;
}

int main(void)
{
	FILE *f;
	char buf[8] = "";
	int x = -1, r, c;

	pc_begin("XSH.vfscanf.001");
	pc_make_file("f", "12 ab rest");
	pc_test();
	f = fopen("f", "r");
	NEED(f != NULL, "fopen(f) failed: %s", pc_errname(errno));
	r = call(f, "%d %s", &x, buf);
	pc_check(r == 2, "vfscanf() returned %d, expected 2", r);
	pc_check(x == 12 && strcmp(buf, "ab") == 0, "stored %d and \"%s\", expected 12 and \"ab\"", x, buf);
	c = fgetc(f);
	pc_check(c == ' ', "the next fgetc() returned %#x, expected the space after \"ab\"", (unsigned)c);
	pc_pass();
}
