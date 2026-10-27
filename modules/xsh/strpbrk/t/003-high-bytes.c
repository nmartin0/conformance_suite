/*
 * strpbrk: high bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strpbrk.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strpbrk)(const char *, const char *) = strpbrk;

int main(void)
{
	static const char s[] = { 'a', (char)0xe9, (char)0xff, 0 }, set1[] = { (char)0xff, 0 }, set2[] = { (char)0xe9, (char)0xff, 0 };
	pc_begin("XSH.strpbrk.003");
	pc_test();
	pc_check(f_strpbrk(s, set1) == s + 2, "the 0xff byte was not found at index 2");
	pc_check(f_strpbrk(s, set2) == s + 1, "the first of two set members was not found at index 1");
	pc_pass();
}
