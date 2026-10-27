/*
 * a64l: more than six.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.005
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	pc_begin("XSH.a64l.005");
	pc_test();
	pc_check(f_a64l("/.....z") == 1, "a64l(\"/.....z\") was not 1");
	pc_check(f_a64l("//////") == 1090785345L, "a64l(\"//////\") was not 1090785345");
	pc_check(f_a64l("//////z") == 1090785345L, "a64l(\"//////z\") was not 1090785345");
	pc_check(f_a64l("//////zzzz") == 1090785345L, "a64l(\"//////zzzz\") was not 1090785345");
	pc_check(f_a64l("zzzzz/.") == 2147483647L, "a64l(\"zzzzz/.\") was not 2147483647");
	pc_pass();
}
