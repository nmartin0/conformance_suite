/*
 * strtok() overwrites only the separator that ends each token.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.002
 * MUTANT    strtok.nul_all_seps
 * MUTANT    strtok.nul_leading
 */
#include "pc.h"
#include <string.h>

static char s[] = "  ab,,cd e  ", sep[] = " ,";
static const char want[13] = { ' ', ' ', 'a', 'b', '\0', ',', 'c', 'd', '\0', 'e', '\0', ' ', '\0' };

int main(void)
{
	int i;

	pc_begin("XSH.strtok.002");
	pc_test();
	(void)strtok(s, sep);
	for (i = 0; i < 4; i++)
		(void)strtok(NULL, sep);
	for (i = 0; i < 13; i++)
		pc_check(s[i] == want[i], "after the sequence s[%d] is %#x, expected %#x",
		    i, (unsigned)(unsigned char)s[i], (unsigned)(unsigned char)want[i]);
	pc_pass();
}
