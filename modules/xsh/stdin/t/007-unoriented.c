/*
 * The standard streams start unoriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stdin.007
 */
#include "pc.h"
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

int main(void)
{
	int a = fwide(stdin, 0), b = fwide(stdout, 0), c = fwide(stderr, 0);

	pc_begin("XSH.stdin.007");
	pc_test();
	pc_check(a == 0, "fwide(stdin, 0) returned %d at program start", a);
	pc_check(b == 0, "fwide(stdout, 0) returned %d at program start", b);
	pc_check(c == 0, "fwide(stderr, 0) returned %d at program start", c);
	pc_pass();
}
