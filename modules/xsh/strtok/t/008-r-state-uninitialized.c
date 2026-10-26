/*
 * strtok_r() with a new string ignores what *state held.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.008
 * MUTANT    strtok.r_uses_state_first
 */
#include "pc.h"
#include <string.h>

static char s[] = "p,q", other[] = "zzz,yyy", comma[] = ",";

int main(void)
{
	char *r[3], *st;

	pc_begin("XSH.strtok.008");
	st = other + 1;
	pc_test();
	r[0] = strtok_r(s, comma, &st);
	r[1] = strtok_r(NULL, comma, &st);
	r[2] = strtok_r(NULL, comma, &st);
	pc_check(r[0] == s, "strtok_r(s, ...) with *state preset did not return s");
	pc_check(r[1] == s + 2, "the next call did not return \"q\" at s+2");
	pc_check(r[2] == NULL, "the call after the last token did not return a null pointer");
	pc_check(memcmp(other, "zzz,yyy", 8) == 0, "the string *state pointed to was changed");
	pc_pass();
}
