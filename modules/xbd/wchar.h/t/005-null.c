/*
 * <wchar.h> defines NULL as a null pointer constant.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.005
 */
#include <wchar.h>
#ifndef NULL
#define NULL_MISSING 1
#endif
#include "pc.h"

#ifndef NULL_MISSING
static wchar_t *sp = NULL;
static const char *cp = NULL;
#endif

int main(void)
{
	pc_begin("XBD.wchar.h.005");
	pc_test();
#ifdef NULL_MISSING
	pc_fail("<wchar.h> does not define NULL");
#else
	{
		wchar_t *p = NULL;
		void *v = NULL;

		pc_check(sp == NULL && cp == NULL && p == NULL && v == NULL, "NULL is not a null pointer");
		pc_check(p == (wchar_t *)0, "NULL differs from a null pointer");
	}
	pc_pass();
#endif
}
