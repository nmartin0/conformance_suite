/*
 * putc.macro.h - conforming variant (CI only): putc() is a macro that
 * evaluates its stream argument twice.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XSH putc(), DESCRIPTION: putc() may be a macro that evaluates stream more
 * than once (c still once, XSH 2.1.1), so a test may pass it only a stream
 * expression without side effects. Here both evaluations are used.
 */
#include <stdio.h>

static inline int pc_variant_putc(int c, FILE *a, FILE *b)
{
	return fputc(c, a == b ? a : b);
}

#undef putc
#define putc(c, s) pc_variant_putc((c), (s), (s))
