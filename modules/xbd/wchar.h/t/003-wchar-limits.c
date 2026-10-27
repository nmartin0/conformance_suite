/*
 * WCHAR_MIN and WCHAR_MAX fit wchar_t's signedness and the minimum
 *   ranges.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.003
 */
#include <wchar.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.wchar.h.003");
	pc_test();
#if !defined(WCHAR_MIN) || !defined(WCHAR_MAX)
	pc_fail("<wchar.h> does not define WCHAR_MIN and WCHAR_MAX");
#else
	pc_check((wchar_t)WCHAR_MIN == WCHAR_MIN && (wchar_t)WCHAR_MAX == WCHAR_MAX, "WCHAR_MIN or WCHAR_MAX is not a wchar_t value");
#if WCHAR_MIN < 0
	pc_check((wchar_t)-1 < 0, "WCHAR_MIN is negative but wchar_t is unsigned");
#if WCHAR_MIN > -127
	pc_fail("WCHAR_MIN is above -127");
#endif
#if WCHAR_MAX < 127
	pc_fail("WCHAR_MAX is below 127");
#endif
#else
	pc_check((wchar_t)-1 > 0, "WCHAR_MIN is not negative but wchar_t is signed");
#if WCHAR_MIN != 0
	pc_fail("wchar_t is unsigned but WCHAR_MIN is not 0");
#endif
#if WCHAR_MAX < 255
	pc_fail("WCHAR_MAX is below 255");
#endif
	pc_check((unsigned long long)(wchar_t)-1 == (unsigned long long)WCHAR_MAX, "WCHAR_MAX is not the largest value of the unsigned wchar_t");
#endif
	pc_pass();
#endif
}
