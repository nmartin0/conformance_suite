/*
 * glob() does not fail because of a large file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.032
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
	glob_t g;
	static const char *const w[] = { "big" };
	int fd, r;

	pc_begin("XSH.glob.032");
	NEED(sizeof(off_t) > 4, "off_t is too small for a large file");
	fd = open("big", O_WRONLY | O_CREAT | O_EXCL, 0600);
	NEED(fd >= 0, "open() failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)0x80000001) == 0, "ftruncate() failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close() failed: %s", pc_errname(errno));
	pc_test();
	r = glob("big*", 0, NULL, &g);
	pc_check(r == 0, "glob() returned %d", r);
	check_list(&g, 0, 1, w);
	globfree(&g);
	/* GLOB_MARK makes glob() look at the file's type, which is where a large file could be refused. */
	r = glob("big*", GLOB_MARK, NULL, &g);
	pc_check(r == 0, "glob(GLOB_MARK) returned %d", r);
	check_list(&g, 0, 1, w);
	globfree(&g);
	pc_pass();
}
