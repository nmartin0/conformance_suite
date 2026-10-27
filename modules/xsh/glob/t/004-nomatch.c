/*
 * A pattern that matches nothing returns GLOB_NOMATCH and a zero count.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.004
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

/* g holds off null pointers, the n names in want in this order, then a null pointer. */
static void check_list(const glob_t *g, size_t off, size_t n, const char *const *want)
{
	size_t i;

	pc_check(g->gl_pathc == n, "gl_pathc is %lu, expected %lu", (unsigned long)g->gl_pathc, (unsigned long)n);
	for (i = 0; i < off; i++)
		pc_check(g->gl_pathv[i] == NULL, "slot %lu before the names is not null", (unsigned long)i);
	for (i = 0; i < n; i++) {
		pc_check(g->gl_pathv[off + i] != NULL, "name %lu is a null pointer", (unsigned long)i);
		pc_check(strcmp(g->gl_pathv[off + i], want[i]) == 0, "name %lu is \"%s\", expected \"%s\"",
		    (unsigned long)i, g->gl_pathv[off + i], want[i]);
	}
	pc_check(g->gl_pathv[off + n] == NULL, "the list does not end in a null pointer");
}

int main(void)
{
	static glob_t g[1];
	int r;

	pc_begin("XSH.glob.004");
	pc_make_file("a", "");
	pc_test();
	r = glob("zz*", 0, NULL, &g[0]);
	pc_check(r == GLOB_NOMATCH, "glob(\"zz*\") returned %d, expected GLOB_NOMATCH", r);
	pc_check(g[0].gl_pathc == 0, "gl_pathc is %lu after GLOB_NOMATCH", (unsigned long)g[0].gl_pathc);
	globfree(&g[0]);
	pc_pass();
}
