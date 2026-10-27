/*
 * a64l: low 32 bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.008
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	pc_begin("XSH.a64l.008");
	if (LONG_MAX <= 2147483647L)
		pc_notinuse("long has only 32 bits");
	pc_test();
	{
		volatile int sh = 32;
		long big = ((long)1 << sh);
		char *s = f_l64a(big + 1);
		pc_check(strcmp(s, "/") == 0, "l64a(2^32 + 1) was \"%s\", expected \"/\"", s);
		pc_check(f_a64l(f_l64a(big + 0x23456789L)) == 0x23456789L, "a64l(l64a(0x123456789)) was not 0x23456789");
	}
	pc_pass();
}
