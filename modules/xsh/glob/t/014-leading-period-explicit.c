/*
 * An explicit period matches a leading period.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.014
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
	glob_t g, h;
	static const char *const w[] = { ".hid" };
	size_t i;
	int r;

	pc_begin("XSH.glob.014");
	pc_make_file(".hid", "");
	pc_make_file("v", "");
	pc_make_dir("d");
	pc_make_file("d/.x", "");
	pc_make_file("d/y", "");
	pc_test();
	r = glob(".h*", 0, NULL, &g);
	pc_check(r == 0, "glob(\".h*\") returned %d", r);
	check_list(&g, 0, 1, w);
	r = glob("d/.*", 0, NULL, &h);
	pc_check(r == 0, "glob(\"d/.*\") returned %d", r);
	pc_check(has(&h, "d/.x"), "d/.x was not found");
	for (i = 0; i < h.gl_pathc; i++)
		pc_check(strcmp(h.gl_pathv[i], "d/.x") == 0 || strcmp(h.gl_pathv[i], "d/.") == 0 ||
		    strcmp(h.gl_pathv[i], "d/..") == 0, "unexpected name \"%s\"", h.gl_pathv[i]);
	globfree(&g);
	globfree(&h);
	pc_pass();
}
