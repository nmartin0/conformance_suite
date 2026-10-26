/*
 * strndup() with size beyond the string copies all of it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strdup.003
 * MUTANT    strdup.n_static
 * MUTANT    strdup.n_drops_last
 * VARIANT   strdup.alloc_size
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char abc[128] = "abc", empty[16];

static void one(char *s, size_t size, const char *want, const char *what)
{
	size_t n = strlen(want);
	char *p = strndup(s, size), *q;

	pc_check(p != NULL, "strndup(%s, %zu) returned a null pointer (%s)", what, size, pc_errname(errno));
	pc_check(p != s, "strndup(%s, %zu) returned its argument", what, size);
	pc_check(memcmp(p, want, n + 1) == 0, "strndup(%s, %zu) gave \"%s\"", what, size, p);
	/* A second copy, with the first still held: new memory each time. */
	q = strndup(s, size);
	pc_check(q != NULL, "a second strndup(%s, %zu) returned a null pointer (%s)", what, size, pc_errname(errno));
	pc_check(q != p, "two strndup(%s, %zu) calls returned the same pointer", what, size);
	pc_check(memcmp(p, want, n + 1) == 0 && memcmp(q, want, n + 1) == 0,
	    "with two copies of %s held, one no longer holds the string", what);
	free(q);
	q = realloc(p, 4096);
	pc_check(q != NULL, "realloc() of the copy of %s failed", what);
	pc_check(memcmp(q, want, n + 1) == 0, "realloc() of the copy of %s lost its bytes", what);
	free(q);
}

int main(void)
{
	pc_begin("XSH.strdup.003");
	memset(abc + 4, 'Z', sizeof abc - 5);	/* bytes after the null byte */
	pc_test();
	one(abc, 4, "abc", "\"abc\"");
	one(abc, 100, "abc", "\"abc\"");
	one(empty, 10, "", "\"\"");
	pc_pass();
}
