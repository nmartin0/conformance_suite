/*
 * a64l: round trip.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.004
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	static const long vals[] = { 0, 1, 63, 64, 4095, 4096, 123456, 1048576, 123456789L, 0x12345678L, 2147483647L };
	size_t k;
	pc_begin("XSH.a64l.004");
	pc_test();
	for (k = 0; k < sizeof vals / sizeof vals[0]; k++)
		pc_check(f_a64l(f_l64a(vals[k])) == vals[k], "a64l(l64a(%ld)) did not give the value back", vals[k]);
	pc_pass();
}
