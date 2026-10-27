/*
 * GLOB_NOSORT returns exactly the matching names in some order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.022
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

static int has(const glob_t *g, const char *s)
{
	size_t i;

	for (i = 0; i < g->gl_pathc; i++)
		if (strcmp(g->gl_pathv[i], s) == 0)
			return 1;
	return 0;
}

int main(void)
{
	glob_t g;
	int r;

	pc_begin("XSH.glob.022");
	pc_make_file("c", "");
	pc_make_file("a", "");
	pc_make_file("b", "");
	pc_test();
	r = glob("*", GLOB_NOSORT, NULL, &g);
	pc_check(r == 0, "glob(GLOB_NOSORT) returned %d", r);
	pc_check(g.gl_pathc == 3, "gl_pathc is %lu, expected 3", (unsigned long)g.gl_pathc);
	pc_check(has(&g, "a") && has(&g, "b") && has(&g, "c"), "a name is missing");
	pc_check(g.gl_pathv[3] == NULL, "the list does not end in a null pointer");
	globfree(&g);
	pc_pass();
}
