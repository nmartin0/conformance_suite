/*
 * a64l: least significant first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.002
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static void enc(unsigned long v, char *o)
{
	while (v) { *o++ = alpha[v & 63]; v >>= 6; }
	*o = '\0';
}

int main(void)
{
	static const unsigned long vals[] = { 64, 65, 4096, 4097, 262144, 2147483647ul, 0x12345678ul, 123456789ul, 1000000ul, 0x40000000ul };
	char want[16];
	size_t k;
	char *s;
	pc_begin("XSH.a64l.002");
	pc_test();
	s = f_l64a(64L);
	pc_check(strcmp(s, "./") == 0, "l64a(64) was \"%s\", expected \"./\"", s);
	s = f_l64a(65L);
	pc_check(strcmp(s, "//") == 0, "l64a(65) was \"%s\", expected \"//\"", s);
	s = f_l64a(4096L);
	pc_check(strcmp(s, "../") == 0, "l64a(4096) was \"%s\", expected \"../\"", s);
	s = f_l64a(4097L);
	pc_check(strcmp(s, "/./") == 0, "l64a(4097) was \"%s\", expected \"/./\"", s);
	s = f_l64a(2147483647L);
	pc_check(strcmp(s, "zzzzz/") == 0, "l64a(2147483647) was \"%s\", expected \"zzzzz/\"", s);
	for (k = 0; k < sizeof vals / sizeof vals[0]; k++) {
		enc(vals[k], want);
		s = f_l64a((long)vals[k]);
		pc_check(strcmp(s, want) == 0, "l64a(%lu) was \"%s\", expected \"%s\"", vals[k], s, want);
		pc_check(strlen(s) <= 6, "l64a(%lu) returned more than six characters", vals[k]);
	}
	pc_pass();
}
