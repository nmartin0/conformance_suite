/*
 * 'm' allocates the result as malloc() would.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.037
 * MUTANT    fscanf.m_not_malloc
 * MUTANT    fscanf.m_uncounted
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
	char *p = NULL, *q = NULL, *s = NULL;
	int r;

	pc_begin("XSH.fscanf.037");
	pc_test();
	r = sscanf("hello abc xyz", "%ms %3mc %m[xyz]", &p, &q, &s);
	pc_check(r == 3, "sscanf() returned %d, expected 3", r);
	pc_check(p != NULL && strcmp(p, "hello") == 0, "%%ms gave \"%s\"", p != NULL ? p : "(null)");
	pc_check(q != NULL && memcmp(q, "abc", 3) == 0, "%%3mc did not give \"abc\"");
	pc_check(s != NULL && strcmp(s, "xyz") == 0, "%%m[xyz] gave \"%s\"", s != NULL ? s : "(null)");
	/* the buffers are the caller's, to release with free() */
	free(p);
	free(q);
	free(s);
	pc_pass();
}
