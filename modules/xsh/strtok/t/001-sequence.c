/*
 * strtok() returns each token in turn, then null pointers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.001
 * MUTANT    strtok.skips_no_leading
 * MUTANT    strtok.no_null_at_end
 */
#include "pc.h"
#include <string.h>

static char s[] = "  ab,,cd e  ", sep[] = " ,";

int main(void)
{
	char *r[5];
	int i;

	pc_begin("XSH.strtok.001");
	pc_test();
	r[0] = strtok(s, sep);
	for (i = 1; i < 5; i++)
		r[i] = strtok(NULL, sep);
	pc_check(r[0] == s + 2, "the first call did not return s+2");
	pc_check(memcmp(r[0], "ab", 3) == 0, "the first token is not \"ab\"");
	pc_check(r[1] == s + 6, "the second call did not return s+6");
	pc_check(memcmp(r[1], "cd", 3) == 0, "the second token is not \"cd\"");
	pc_check(r[2] == s + 9, "the third call did not return s+9");
	pc_check(memcmp(r[2], "e", 2) == 0, "the third token is not \"e\"");
	pc_check(r[3] == NULL, "the fourth call (only separators left) did not return a null pointer");
	pc_check(r[4] == NULL, "the fifth call did not return a null pointer");
	pc_pass();
}
