/*
 * strndup() with size below the length copies size bytes and a null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strdup.004
 * MUTANT    strdup.n_ignores_size
 * MUTANT    strdup.n_no_terminator
 * MUTANT    strdup.n_drops_last
 * VARIANT   strdup.alloc_size
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char s[] = "abcdef";

int main(void)
{
	char *p;

	pc_begin("XSH.strdup.004");
	pc_test();
	p = strndup(s, 3);
	pc_check(p != NULL, "strndup(\"abcdef\", 3) returned a null pointer (%s)", pc_errname(errno));
	pc_check(memcmp(p, "abc", 4) == 0, "strndup(\"abcdef\", 3) gave \"%.10s\"", p);
	free(p);
	p = strndup(s, 0);
	pc_check(p != NULL, "strndup(\"abcdef\", 0) returned a null pointer (%s)", pc_errname(errno));
	pc_check(p[0] == '\0', "strndup(\"abcdef\", 0) gave \"%.10s\"", p);
	free(p);
	pc_pass();
}
