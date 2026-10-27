/*
 * CHAR_BIT, SCHAR_*, UCHAR_MAX and the CHAR_MIN/CHAR_MAX choice.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.006
 */
#include <limits.h>
#include "pc.h"
#if !defined(CHAR_BIT) || (CHAR_BIT) != 8
#error "CHAR_BIT is missing or not the required value"
#endif
#if !defined(SCHAR_MAX) || (SCHAR_MAX) != 127
#error "SCHAR_MAX is missing or not the required value"
#endif
#if !defined(SCHAR_MIN) || (SCHAR_MIN) != (-128)
#error "SCHAR_MIN is missing or not the required value"
#endif
#if !defined(UCHAR_MAX) || (UCHAR_MAX) != 255
#error "UCHAR_MAX is missing or not the required value"
#endif

#if CHAR_MIN < 0
#if CHAR_MIN != SCHAR_MIN || CHAR_MAX != SCHAR_MAX
#error "char is signed but CHAR_MIN/CHAR_MAX differ from SCHAR_MIN/SCHAR_MAX"
#endif
#else
#if CHAR_MIN != 0 || CHAR_MAX != UCHAR_MAX
#error "char is unsigned but CHAR_MIN is not 0 or CHAR_MAX is not UCHAR_MAX"
#endif
#endif

int main(void)
{
	pc_begin("XBD.limits.h.006");
	pc_test();
	/* char can hold negative values exactly when CHAR_MIN is negative */
	pc_check(((char)-1 < 0) == (CHAR_MIN < 0), "CHAR_MIN does not match the signedness of char");
	pc_pass();
}
