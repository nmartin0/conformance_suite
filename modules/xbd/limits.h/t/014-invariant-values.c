/*
 * The Other Invariant Values are defined and not below their minima.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.014
 */
#include <limits.h>
#include "pc.h"
#ifndef GETENTROPY_MAX
#error "GETENTROPY_MAX is not defined"
#endif
#if defined(GETENTROPY_MAX) && (GETENTROPY_MAX) < (256)
#error "GETENTROPY_MAX is below its minimum acceptable value"
#endif
#ifndef NL_ARGMAX
#error "NL_ARGMAX is not defined"
#endif
#if defined(NL_ARGMAX) && (NL_ARGMAX) < (9)
#error "NL_ARGMAX is below its minimum acceptable value"
#endif
#ifndef NL_MSGMAX
#error "NL_MSGMAX is not defined"
#endif
#if defined(NL_MSGMAX) && (NL_MSGMAX) < (32767)
#error "NL_MSGMAX is below its minimum acceptable value"
#endif
#ifndef NL_SETMAX
#error "NL_SETMAX is not defined"
#endif
#if defined(NL_SETMAX) && (NL_SETMAX) < (255)
#error "NL_SETMAX is below its minimum acceptable value"
#endif
#ifndef NL_TEXTMAX
#error "NL_TEXTMAX is not defined"
#endif
#if defined(NL_TEXTMAX) && (NL_TEXTMAX) < (_POSIX2_LINE_MAX)
#error "NL_TEXTMAX is below its minimum acceptable value"
#endif
#ifndef NSIG_MAX
#error "NSIG_MAX is not defined"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.014");
	pc_test();
	pc_pass();
}
