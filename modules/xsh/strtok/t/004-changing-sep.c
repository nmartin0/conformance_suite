/*
 * strtok() uses the separators given on each call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.004
 * MUTANT    strtok.fixed_sep
 */
#include "pc.h"
#include <string.h>

static char s[] = "a,b c,d", comma[] = ",", space[] = " ";

int main(void)
{
	char *r[5];

	pc_begin("XSH.strtok.004");
	pc_test();
	r[0] = strtok(s, comma);
	r[1] = strtok(NULL, space);
	r[2] = strtok(NULL, comma);
	r[3] = strtok(NULL, comma);
	r[4] = strtok(NULL, comma);
	pc_check(r[0] == s && memcmp(r[0], "a", 2) == 0, "the first call did not return \"a\" at s");
	pc_check(r[1] == s + 2 && memcmp(r[1], "b", 2) == 0, "the call with \" \" did not return \"b\" at s+2");
	pc_check(r[2] == s + 4 && memcmp(r[2], "c", 2) == 0, "the third call did not return \"c\" at s+4");
	pc_check(r[3] == s + 6 && memcmp(r[3], "d", 2) == 0, "the fourth call did not return \"d\" at s+6");
	pc_check(r[4] == NULL, "the fifth call did not return a null pointer");
	pc_pass();
}
