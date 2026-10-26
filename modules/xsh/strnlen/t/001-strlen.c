/*
 * strlen() counts the bytes before the null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strnlen.001
 * MUTANT    strnlen.strlen_counts_nul
 * MUTANT    strnlen.strlen_stops_high
 */
#include "pc.h"
#include <string.h>

static char empty[1], a[] = "a", high[] = "x\x80y\xffz!", big[4096];

int main(void)
{
	size_t r[4];

	pc_begin("XSH.strnlen.001");
	memset(big, 'b', 4095);
	pc_test();
	r[0] = strlen(empty);
	r[1] = strlen(a);
	r[2] = strlen(big);
	r[3] = strlen(high);
	pc_check(r[0] == 0, "strlen(\"\") returned %zu", r[0]);
	pc_check(r[1] == 1, "strlen(\"a\") returned %zu", r[1]);
	pc_check(r[2] == 4095, "strlen() of a 4095-byte string returned %zu", r[2]);
	pc_check(r[3] == 6, "strlen() of a 6-byte string with bytes 0x80 and 0xff returned %zu", r[3]);
	pc_pass();
}
