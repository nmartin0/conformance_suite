/*
 * a64l: sign extension.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.007
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	pc_begin("XSH.a64l.007");
	pc_test();
	pc_check(f_a64l("zzzzz1") == -1L, "a64l(\"zzzzz1\") was not -1");
	pc_check(f_a64l(".....0") == -2147483647L - 1L, "a64l(\".....0\") was not -2147483648");
	if (LONG_MAX > 2147483647L)
		pc_check(f_a64l("zzzzzz") == -1L, "a64l(\"zzzzzz\") was not -1");
	pc_check(f_a64l("zzzzz.") == 1073741823L, "a64l(\"zzzzz.\") was not 1073741823");
	pc_pass();
}
