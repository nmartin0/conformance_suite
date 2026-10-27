/*
 * A pattern without wildcards finds an existing file and returns GLOB_NOMATCH for a missing one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.005
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
	static glob_t g[2];
	static const char *const w[][8] = {
		{"lit"},
		{0},
	};
	int r;

	pc_begin("XSH.glob.005");
	pc_make_file("lit", "");
	pc_test();
	r = glob("lit", 0, NULL, &g[0]);
	pc_check(r == 0, "glob(\"lit\") returned %d, expected 0", r);
	check_list(&g[0], 0, 1, w[0]);
	r = glob("missing", 0, NULL, &g[1]);
	pc_check(r == GLOB_NOMATCH, "glob(\"missing\") returned %d, expected GLOB_NOMATCH", r);
	pc_check(g[1].gl_pathc == 0, "gl_pathc is %lu after GLOB_NOMATCH", (unsigned long)g[1].gl_pathc);
	globfree(&g[0]);
	globfree(&g[1]);
	pc_pass();
}
