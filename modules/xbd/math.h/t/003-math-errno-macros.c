/*
 * MATH_ERRNO is 1 and MATH_ERREXCEPT is 2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.003
 */
#include <math.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.math.h.003");
	pc_test();
#if !defined(MATH_ERRNO) || !defined(MATH_ERREXCEPT)
	pc_fail("<math.h> does not define MATH_ERRNO and MATH_ERREXCEPT");
#elif MATH_ERRNO != 1
	pc_fail("MATH_ERRNO is not 1");
#elif MATH_ERREXCEPT != 2
	pc_fail("MATH_ERREXCEPT is not 2");
#else
	pc_pass();
#endif
}
