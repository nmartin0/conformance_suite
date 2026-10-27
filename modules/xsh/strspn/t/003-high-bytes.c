/*
 * strspn: high bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strspn.003
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strspn)(const char *, const char *) = strspn;

int main(void)
{
	static const char s1[] = { (char)0xe9, (char)0xe9, 'a', 0 }, s2[] = { (char)0xff, 0 }, set[] = { (char)0xe9, 0 };
	pc_begin("XSH.strspn.003");
	pc_test();
	pc_check(f_strspn(s1, set) == 2, "two members at the start gave the wrong length");
	pc_check(f_strspn(s2, set) == 0, "a different high byte was taken as a member");
	pc_pass();
}
