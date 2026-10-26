/*
 * strtok_r() tokenizes like strtok(), keeping its state in *state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.006
 * MUTANT    strtok.r_uses_static
 * MUTANT    strtok.r_skips_no_leading
 */
#include "pc.h"
#include <string.h>

static char s[] = "  ab,,cd e  ", sep[] = " ,", a[] = "a b c", b[] = "1,2,3", sp[] = " ", comma[] = ",";

int main(void)
{
	char *r[5], *ra[4], *rb[4], *st, *sa, *sb;
	int i;

	pc_begin("XSH.strtok.006");
	pc_test();
	r[0] = strtok_r(s, sep, &st);
	for (i = 1; i < 5; i++)
		r[i] = strtok_r(NULL, sep, &st);
	ra[0] = strtok_r(a, sp, &sa);
	rb[0] = strtok_r(b, comma, &sb);
	for (i = 1; i < 4; i++) {
		ra[i] = strtok_r(NULL, sp, &sa);
		rb[i] = strtok_r(NULL, comma, &sb);
	}
	pc_check(r[0] == s + 2 && r[1] == s + 6 && r[2] == s + 9,
	    "strtok_r() did not return s+2, s+6 and s+9");
	pc_check(memcmp(r[0], "ab", 3) == 0 && memcmp(r[1], "cd", 3) == 0 && memcmp(r[2], "e", 2) == 0,
	    "strtok_r() did not give \"ab\", \"cd\" and \"e\"");
	pc_check(r[3] == NULL && r[4] == NULL, "strtok_r() did not return null pointers after the last token");
	for (i = 0; i < 3; i++) {
		pc_check(ra[i] == a + 2 * i, "token %d of \"a b c\" is not at a+%d", i + 1, 2 * i);
		pc_check(rb[i] == b + 2 * i, "token %d of \"1,2,3\" is not at b+%d", i + 1, 2 * i);
	}
	pc_check(memcmp(a, "a\0b\0c", 6) == 0 && memcmp(b, "1\0" "2\0" "3", 6) == 0,
	    "the alternating scans left wrong bytes");
	pc_check(ra[3] == NULL && rb[3] == NULL, "the alternating scans did not end with null pointers");
	pc_pass();
}
