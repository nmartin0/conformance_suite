/*
 * a64l: digit alphabet.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	long v;
	char *s, one[2];
	pc_begin("XSH.a64l.001");
	pc_test();
	s = f_l64a(0L);
	pc_check(s != NULL && s[0] == '\0', "l64a(0) was not an empty string");
	for (v = 1; v < 64; v++) {
		s = f_l64a(v);
		pc_check(s != NULL, "l64a(%ld) returned a null pointer", v);
		one[0] = alpha[v]; one[1] = '\0';
		pc_check(strcmp(s, one) == 0, "l64a(%ld) was \"%s\", expected \"%s\"", v, s, one);
	}
	pc_pass();
}
