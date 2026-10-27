/*
 * LC_ALL, LC_COLLATE, LC_CTYPE, LC_MESSAGES, LC_MONETARY, LC_NUMERIC and
 *   LC_TIME are integer constant expressions with distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.locale.h.001
 */
#include <locale.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.locale.h.001");
	pc_test();
#if !defined(LC_ALL) || !defined(LC_COLLATE) || !defined(LC_CTYPE) || !defined(LC_MESSAGES) || !defined(LC_MONETARY) || !defined(LC_NUMERIC) || !defined(LC_TIME)
	pc_fail("<locale.h> does not define every LC_ category macro");
#elif LC_ALL == LC_COLLATE || \
    LC_ALL == LC_CTYPE || \
    LC_ALL == LC_MESSAGES || \
    LC_ALL == LC_MONETARY || \
    LC_ALL == LC_NUMERIC || \
    LC_ALL == LC_TIME || \
    LC_COLLATE == LC_CTYPE || \
    LC_COLLATE == LC_MESSAGES || \
    LC_COLLATE == LC_MONETARY || \
    LC_COLLATE == LC_NUMERIC || \
    LC_COLLATE == LC_TIME || \
    LC_CTYPE == LC_MESSAGES || \
    LC_CTYPE == LC_MONETARY || \
    LC_CTYPE == LC_NUMERIC || \
    LC_CTYPE == LC_TIME || \
    LC_MESSAGES == LC_MONETARY || \
    LC_MESSAGES == LC_NUMERIC || \
    LC_MESSAGES == LC_TIME || \
    LC_MONETARY == LC_NUMERIC || \
    LC_MONETARY == LC_TIME || \
    LC_NUMERIC == LC_TIME
	pc_fail("two of the LC_ category macros have the same value");
#else
	pc_pass();
#endif
}
