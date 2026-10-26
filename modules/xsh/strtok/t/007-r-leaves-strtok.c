/*
 * strtok_r() calls do not disturb a strtok() sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.007
 * MUTANT    strtok.r_clobbers_strtok
 */
#include "pc.h"
#include <string.h>

static char a[] = "a b c", b[] = "1,2,3", sp[] = " ", comma[] = ",";

int main(void)
{
	char *r[4], *st;

	pc_begin("XSH.strtok.007");
	pc_test();
	r[0] = strtok(a, sp);
	if (strtok_r(b, comma, &st) != NULL)
		while (strtok_r(NULL, comma, &st) != NULL)
			;
	r[1] = strtok(NULL, sp);
	r[2] = strtok(NULL, sp);
	r[3] = strtok(NULL, sp);
	pc_check(r[0] == a, "strtok() did not return \"a\" at s");
	pc_check(r[1] == a + 2, "after a strtok_r() scan strtok() did not continue with \"b\" at s+2");
	pc_check(r[2] == a + 4, "strtok() did not continue with \"c\" at s+4");
	pc_check(r[3] == NULL, "strtok() did not end with a null pointer");
	pc_pass();
}
