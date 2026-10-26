/*
 * strtok() with no separators returns the whole string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.005
 * MUTANT    strtok.empty_sep_null
 * MUTANT    strtok.no_null_at_end
 */
#include "pc.h"
#include <string.h>

static char s[] = " ab c", none[1];

int main(void)
{
	char *r0, *r1;

	pc_begin("XSH.strtok.005");
	pc_test();
	r0 = strtok(s, none);
	r1 = strtok(NULL, none);
	pc_check(r0 == s, "strtok(s, \"\") did not return s");
	pc_check(memcmp(s, " ab c", 6) == 0, "strtok(s, \"\") changed the string");
	pc_check(r1 == NULL, "the next call did not return a null pointer");
	pc_pass();
}
