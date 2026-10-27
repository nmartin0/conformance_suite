/*
 * XSI systems provide the pointer-holding types and their limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.006
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"

#if !defined(INTPTR_MIN) || !defined(INTPTR_MAX) || !defined(UINTPTR_MAX)
#error "INTPTR_MIN, INTPTR_MAX or UINTPTR_MAX is not defined"
#endif
static intptr_t s;
static uintptr_t u;

int main(void)
{
	pc_begin("XBD.stdint.h.006");
	pc_test();
	s = 1;
	u = 1;
	pc_check(s == 1 && u == 1, "intptr_t or uintptr_t cannot hold 1");
	pc_pass();
}
