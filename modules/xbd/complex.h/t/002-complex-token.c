/*
 * complex expands to _Complex and imaginary, when defined, to
 *   _Imaginary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.002
 */
#include <complex.h>
#include <string.h>
#include "pc.h"


#define STR(x) #x
#define XSTR(x) STR(x)

int main(void)
{
	pc_begin("XBD.complex.h.002");
	pc_test();
#ifndef complex
	pc_fail("<complex.h> does not define complex as a macro");
#else
	pc_check(strcmp(XSTR(complex), "_Complex") == 0, "complex expands to '%s', not _Complex", XSTR(complex));
#endif
#ifdef imaginary
	pc_check(strcmp(XSTR(imaginary), "_Imaginary") == 0, "imaginary expands to '%s', not _Imaginary", XSTR(imaginary));
#endif
	pc_pass();
}
