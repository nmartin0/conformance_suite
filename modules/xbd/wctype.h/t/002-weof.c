/*
 * <wctype.h> defines WEOF with the same value and type as <wchar.h>.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wctype.h.002
 */
#include <wctype.h>
#ifndef WEOF
#define WEOF_MISSING 1
#else
static const wint_t first = WEOF;
#endif
#include <wchar.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.wctype.h.002");
	pc_test();
#ifdef WEOF_MISSING
	pc_fail("<wctype.h> does not define WEOF");
#else
	pc_check(IS_T(WEOF, wint_t), "WEOF does not have type wint_t");
	pc_check(first == WEOF, "<wctype.h> and <wchar.h> give WEOF different values");
	pc_pass();
#endif
}
