/*
 * Limits of ptrdiff_t, size_t, sig_atomic_t, wchar_t and wint_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.008
 */
#include <stdint.h>
#include <stddef.h>
#include <signal.h>
#include <wchar.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if PTRDIFF_MIN > -65536 || PTRDIFF_MAX < 65535 || SIZE_MAX < 65535
#error "PTRDIFF_MIN, PTRDIFF_MAX or SIZE_MAX is below the required magnitude"
#endif
_Static_assert(TYID(PTRDIFF_MIN) == TYID(+(ptrdiff_t)0) && TYID(PTRDIFF_MAX) == TYID(+(ptrdiff_t)0), "PTRDIFF_MIN/MAX do not have the promoted type");
_Static_assert(TYID(SIZE_MAX) == TYID(+(size_t)0), "SIZE_MAX does not have the promoted type");
_Static_assert(TYID(SIG_ATOMIC_MIN) == TYID(+(sig_atomic_t)0) && TYID(SIG_ATOMIC_MAX) == TYID(+(sig_atomic_t)0), "SIG_ATOMIC_MIN/MAX do not have the promoted type");
_Static_assert(TYID(WCHAR_MIN) == TYID(+(wchar_t)0) && TYID(WCHAR_MAX) == TYID(+(wchar_t)0), "WCHAR_MIN/MAX do not have the promoted type");
_Static_assert(TYID(WINT_MIN) == TYID(+(wint_t)0) && TYID(WINT_MAX) == TYID(+(wint_t)0), "WINT_MIN/MAX do not have the promoted type");
_Static_assert((ptrdiff_t)-1 < 0, "ptrdiff_t is not signed");
_Static_assert((size_t)-1 > 0, "size_t is not unsigned");
_Static_assert(((sig_atomic_t)-1 < (sig_atomic_t)1) ? (SIG_ATOMIC_MIN <= -127 && SIG_ATOMIC_MAX >= 127) : (SIG_ATOMIC_MIN == 0 && SIG_ATOMIC_MAX >= 255),
	"SIG_ATOMIC_MIN/MAX do not fit the signedness of sig_atomic_t");
_Static_assert(((wchar_t)-1 < (wchar_t)1) ? (WCHAR_MIN <= -127 && WCHAR_MAX >= 127) : (WCHAR_MIN == 0 && WCHAR_MAX >= 255),
	"WCHAR_MIN/MAX do not fit the signedness of wchar_t");
_Static_assert(((wint_t)-1 < (wint_t)1) ? (WINT_MIN <= -32767 && WINT_MAX >= 32767) : (WINT_MIN == 0 && WINT_MAX >= 65535),
	"WINT_MIN/MAX do not fit the signedness of wint_t");

int main(void)
{
	pc_begin("XBD.stdint.h.008");
	pc_test();
	pc_pass();
}
