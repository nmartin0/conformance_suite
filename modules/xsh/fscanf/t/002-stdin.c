/*
 * scanf() reads from standard input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.002
 * MUTANT    fscanf.own_buffer
 * MUTANT    fscanf.item_next_lost
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
	int a = -1, r, c, fd;

	pc_begin("XSH.fscanf.002");
	pc_make_file("in", "42 x");
	fd = open("in", O_RDONLY);
	pc_check(fd >= 0, "open(in) failed: %s", pc_errname(errno));
	pc_check(dup2(fd, 0) == 0, "dup2() onto 0 failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_test();
	r = scanf("%d", &a);
	pc_check(r == 1, "scanf() returned %d, expected 1", r);
	pc_check(a == 42, "scanf() stored %d, expected 42", a);
	c = getchar();
	pc_check(c == ' ', "the next getchar() returned %#x, expected the space", (unsigned)c);
	pc_pass();
}
