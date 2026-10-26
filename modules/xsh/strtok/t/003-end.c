/*
 * strtok(): a last token running to the end, and strings with no token.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.003
 * MUTANT    strtok.no_null_at_end
 * MUTANT    strtok.skips_no_leading
 */
#include "pc.h"
#include <string.h>

static char s[] = "x;y", empty[1], seps[] = ";;;", sep[] = ";";

int main(void)
{
	char *r[4], *e, *f;
	int i;

	pc_begin("XSH.strtok.003");
	pc_test();
	r[0] = strtok(s, sep);
	for (i = 1; i < 4; i++)
		r[i] = strtok(NULL, sep);
	e = strtok(empty, sep);
	f = strtok(seps, sep);
	pc_check(r[0] == s && memcmp(s, "x", 2) == 0, "the first call did not return \"x\" at s");
	pc_check(r[1] == s + 2 && memcmp(s + 2, "y", 2) == 0, "the second call did not return \"y\" at s+2");
	pc_check(r[2] == NULL, "the call after the last token did not return a null pointer");
	pc_check(r[3] == NULL, "the next call did not return a null pointer");
	pc_check(e == NULL, "strtok(\"\") did not return a null pointer");
	pc_check(f == NULL, "strtok(\";;;\", \";\") did not return a null pointer");
	pc_pass();
}
