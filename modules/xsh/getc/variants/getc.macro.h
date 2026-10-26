/*
 * getc.macro.h - conforming variant (CI only): getc() is a macro that
 * evaluates its stream argument twice.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XSH getc(), DESCRIPTION: getc() may be a macro that evaluates stream more
 * than once, so a test may pass it only an expression without side
 * effects. Here both evaluations are used.
 */
#include <stdio.h>

static inline int pc_variant_getc(FILE *a, FILE *b)
{
	return fgetc(a == b ? a : b);
}

#undef getc
#define getc(s) pc_variant_getc((s), (s))
