/*
 * strdup() returns a new, freeable copy of the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strdup.001
 * MUTANT    strdup.same_pointer
 * MUTANT    strdup.no_nul
 * MUTANT    strdup.static_buf
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char empty[1], abc[] = "abc", big[3001];

static void one(char *s, const char *what)
{
	size_t n = strlen(s);
	char *p = strdup(s), *q;

	pc_check(p != NULL, "strdup(%s) returned a null pointer (%s)", what, pc_errname(errno));
	pc_check(p != s, "strdup(%s) returned its argument", what);
	pc_check(memcmp(p, s, n + 1) == 0, "strdup(%s) is not an equal string", what);
	if (n > 0) {
		char was = s[0];

		p[0] = (char)(was == 'q' ? 'r' : 'q');
		pc_check(s[0] == was, "changing the copy of %s changed the original", what);
		p[0] = was;
	}
	/* A second copy, with the first still held: a new string each time. */
	q = strdup(s);
	pc_check(q != NULL, "a second strdup(%s) returned a null pointer (%s)", what, pc_errname(errno));
	pc_check(q != p, "two strdup(%s) calls returned the same pointer", what);
	pc_check(memcmp(p, s, n + 1) == 0 && memcmp(q, s, n + 1) == 0,
	    "with two copies of %s held, one no longer holds the string", what);
	free(p);
	free(q);
}

int main(void)
{
	size_t i;

	pc_begin("XSH.strdup.001");
	for (i = 0; i < 3000; i++)
		big[i] = (char)(unsigned char)(1 + i % 255);
	pc_test();
	one(empty, "\"\"");
	one(abc, "\"abc\"");
	one(big, "a 3000-byte string");
	pc_pass();
}
