/*
 * stdin, stdout and stderr are FILE pointers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.006
 */
#include <stdio.h>
#include "pc.h"

_Static_assert(_Generic(stdin, FILE *: 1, default: 0), "stdin is not a FILE *");
_Static_assert(_Generic(stdout, FILE *: 1, default: 0), "stdout is not a FILE *");
_Static_assert(_Generic(stderr, FILE *: 1, default: 0), "stderr is not a FILE *");

int main(void)
{
	pc_begin("XBD.stdio.h.006");
	pc_test();
	pc_check(stdin != NULL && stdout != NULL && stderr != NULL, "a standard stream pointer is null");
	pc_check(stdin != stdout && stdin != stderr && stdout != stderr, "two standard streams are the same object");
	pc_pass();
}
