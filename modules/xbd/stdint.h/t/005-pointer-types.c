/*
 * Pointer-holding integer types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.005
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if defined(INTPTR_MAX) || defined(UINTPTR_MAX)
#if INTPTR_MIN > (-32767 - 1) || INTPTR_MAX < 32767 || UINTPTR_MAX < 65535
#error "a pointer-integer limit is too small in magnitude"
#endif
_Static_assert(TYID(INTPTR_MIN) == TYID(+(intptr_t)0) && TYID(INTPTR_MAX) == TYID(+(intptr_t)0) && TYID(UINTPTR_MAX) == TYID(+(uintptr_t)0),
	"a pointer-integer limit does not have the promoted type");
_Static_assert((intptr_t)-1 < 0, "intptr_t is not signed");
_Static_assert((uintptr_t)-1 > 0, "uintptr_t is not unsigned");
#endif

int main(void)
{
	pc_begin("XBD.stdint.h.005");
	pc_test();
#if defined(INTPTR_MAX) && defined(UINTPTR_MAX)
	{
		static int x, y[3];
		size_t i;
		void *const v[] = { &x, y, y + 2, NULL, (void *)&i };

		for (i = 0; i < sizeof v / sizeof v[0]; i++) {
			intptr_t s = (intptr_t)v[i];
			uintptr_t u = (uintptr_t)v[i];

			pc_check((void *)s == v[i], "pointer %zu does not survive a trip through intptr_t", i);
			pc_check((void *)u == v[i], "pointer %zu does not survive a trip through uintptr_t", i);
		}
	}
	pc_pass();
#else
	pc_notinuse("intptr_t and uintptr_t are not provided (optional unless XSI)");
#endif
}
