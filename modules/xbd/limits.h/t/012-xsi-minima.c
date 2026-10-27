/*
 * XSI limits: minimum values, PAGE_SIZE and PAGESIZE, NL_LANGMAX, NZERO.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.012
 */
#include <limits.h>
#include "pc.h"
#if defined(IOV_MAX) && (IOV_MAX) < (_XOPEN_IOV_MAX)
#error "IOV_MAX is below its minimum acceptable value"
#endif
#if defined(NAME_MAX) && (NAME_MAX) < (_XOPEN_NAME_MAX)
#error "NAME_MAX is below its minimum acceptable value"
#endif
#if defined(PATH_MAX) && (PATH_MAX) < (_XOPEN_PATH_MAX)
#error "PATH_MAX is below its minimum acceptable value"
#endif
#if defined(TEXTDOMAIN_MAX) && (TEXTDOMAIN_MAX) < (_XOPEN_NAME_MAX - 3)
#error "TEXTDOMAIN_MAX is below its minimum acceptable value"
#endif
#ifndef NL_LANGMAX
#error "NL_LANGMAX is not defined"
#endif
#if defined(NL_LANGMAX) && (NL_LANGMAX) < (14)
#error "NL_LANGMAX is below its minimum acceptable value"
#endif
#ifndef NZERO
#error "NZERO is not defined"
#endif
#if defined(NZERO) && (NZERO) < (20)
#error "NZERO is below its minimum acceptable value"
#endif

#if defined(PAGESIZE) || defined(PAGE_SIZE)
#if !defined(PAGESIZE) || !defined(PAGE_SIZE)
#error "only one of PAGESIZE and PAGE_SIZE is defined"
#endif
#if PAGESIZE != PAGE_SIZE
#error "PAGESIZE and PAGE_SIZE differ"
#endif
#endif

int main(void)
{
	pc_begin("XBD.limits.h.012");
	pc_test();
	pc_pass();
}
