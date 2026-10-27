/*
 * __STDC_NO_COMPLEX__ is not defined, except by a subprofile
 *   implementation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.001
 */
#include <unistd.h>

#ifdef __STDC_NO_COMPLEX__
#define NO_COMPLEX_BEFORE 1
#endif

#include <complex.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.complex.h.001");
	pc_test();
#ifdef _POSIX_SUBPROFILE
	pc_notinuse("a subprofile implementation may define __STDC_NO_COMPLEX__");
#elif defined(NO_COMPLEX_BEFORE)
	pc_fail("__STDC_NO_COMPLEX__ is defined before <complex.h> is included");
#elif defined(__STDC_NO_COMPLEX__)
	pc_fail("__STDC_NO_COMPLEX__ is defined after <complex.h> is included");
#else
	pc_pass();
#endif
}
