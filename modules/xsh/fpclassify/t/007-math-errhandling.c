/*
 * math_errhandling selects errno, exception flags or both.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.007
 */
#include <fenv.h>
/* Recorded before mt.h, which supplies placeholder values for absent FE_ macros. */
#if defined(FE_DIVBYZERO) && defined(FE_INVALID) && defined(FE_OVERFLOW)
#define HAVE_FE_ERRORS 1
#else
#define HAVE_FE_ERRORS 0
#endif
#include "mt.h"

int main(void)
{
	pc_begin("XSH.fpclassify.007");
	pc_test();
	enum { e_errno = MATH_ERRNO, e_exc = MATH_ERREXCEPT };
	volatile int h = math_errhandling;

	pc_check(e_errno == 1, "MATH_ERRNO is %d", (int)e_errno);
	pc_check(e_exc == 2, "MATH_ERREXCEPT is %d", (int)e_exc);
	pc_check(h == 1 || h == 2 || h == 3, "math_errhandling is %d", (int)h);
	#if !HAVE_FE_ERRORS
	pc_check((h & MATH_ERREXCEPT) == 0, "math_errhandling selects exception flags but FE_DIVBYZERO, FE_INVALID and FE_OVERFLOW are not all defined");
	#endif
	pc_pass();
}
