/*
 * LC_GLOBAL_LOCALE is a locale_t value that is not a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.locale.h.003
 */
#include <locale.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	locale_t g;

	pc_begin("XBD.locale.h.003");
	pc_test();
#if !defined(LC_GLOBAL_LOCALE)
	pc_fail("<locale.h> does not define LC_GLOBAL_LOCALE");
#else
	pc_check(IS_T(LC_GLOBAL_LOCALE, locale_t), "LC_GLOBAL_LOCALE does not have type locale_t");
	g = LC_GLOBAL_LOCALE;
	pc_check(g != (locale_t)0, "LC_GLOBAL_LOCALE is a null locale_t");
	pc_pass();
#endif
}
