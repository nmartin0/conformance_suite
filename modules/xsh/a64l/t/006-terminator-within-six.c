/*
 * a64l: terminator within six.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.006
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	static const char s1[7] = { '/', 0, '/', '/', '/', '/', '/' }, s2[4] = { '/', '/', 0, '/' }, s3[2] = { 0, 'z' };
	pc_begin("XSH.a64l.006");
	pc_test();
	pc_check(f_a64l(s1) == 1, "a64l({'/', 0, ...}) was not 1");
	pc_check(f_a64l(s2) == 65, "a64l({'/', '/', 0, ...}) was not 65");
	pc_check(f_a64l(s3) == 0, "a64l({0, 'z'}) was not 0");
	pc_pass();
}
