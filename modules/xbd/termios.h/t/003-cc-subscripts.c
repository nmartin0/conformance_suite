/*
 * VEOF, VEOL, VERASE, VINTR, VKILL, VMIN, VQUIT, VSTART, VSTOP, VSUSP and
 * VTIME are usable in #if, non-negative and below NCCS, and different from
 * each other except that VMIN may equal VEOF and VTIME may equal VEOL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.003
 */
#include "pc.h"
#include <string.h>
#include <termios.h>

#ifdef VEOF
#if !((VEOF) || 1)
#error "VEOF is not usable in #if"
#endif
#endif
#ifdef VEOL
#if !((VEOL) || 1)
#error "VEOL is not usable in #if"
#endif
#endif
#ifdef VERASE
#if !((VERASE) || 1)
#error "VERASE is not usable in #if"
#endif
#endif
#ifdef VINTR
#if !((VINTR) || 1)
#error "VINTR is not usable in #if"
#endif
#endif
#ifdef VKILL
#if !((VKILL) || 1)
#error "VKILL is not usable in #if"
#endif
#endif
#ifdef VMIN
#if !((VMIN) || 1)
#error "VMIN is not usable in #if"
#endif
#endif
#ifdef VQUIT
#if !((VQUIT) || 1)
#error "VQUIT is not usable in #if"
#endif
#endif
#ifdef VSTART
#if !((VSTART) || 1)
#error "VSTART is not usable in #if"
#endif
#endif
#ifdef VSTOP
#if !((VSTOP) || 1)
#error "VSTOP is not usable in #if"
#endif
#endif
#ifdef VSUSP
#if !((VSUSP) || 1)
#error "VSUSP is not usable in #if"
#endif
#endif
#ifdef VTIME
#if !((VTIME) || 1)
#error "VTIME is not usable in #if"
#endif
#endif

static const char *const allowed[][2] = {
	{ "VMIN", "VEOF" },
	{ "VTIME", "VEOL" },
};

static int is_allowed(const char *a, const char *b)
{
	unsigned k;

	for (k = 0; k < sizeof allowed / sizeof allowed[0]; k++)
		if ((!strcmp(a, allowed[k][0]) && !strcmp(b, allowed[k][1])) || (!strcmp(a, allowed[k][1]) && !strcmp(b, allowed[k][0])))
			return 1;
	return 0;
}

int main(void)
{
	pc_begin("XBD.termios.h.003");
#if !defined(VEOF) || !defined(VEOL) || !defined(VERASE) || !defined(VINTR) || !defined(VKILL) || !defined(VMIN) || !defined(VQUIT) || !defined(VSTART) || !defined(VSTOP) || !defined(VSUSP) || !defined(VTIME)
	pc_unresolved("a constant is missing (the header check reports it): VEOF VEOL VERASE VINTR VKILL VMIN VQUIT VSTART VSTOP VSUSP VTIME");
#else
	{
	struct { const char *n; long long v; } e[12];
	int n = 0, i, j;

	e[n].n = "VEOF"; e[n++].v = (long long)(VEOF);
	e[n].n = "VEOL"; e[n++].v = (long long)(VEOL);
	e[n].n = "VERASE"; e[n++].v = (long long)(VERASE);
	e[n].n = "VINTR"; e[n++].v = (long long)(VINTR);
	e[n].n = "VKILL"; e[n++].v = (long long)(VKILL);
	e[n].n = "VMIN"; e[n++].v = (long long)(VMIN);
	e[n].n = "VQUIT"; e[n++].v = (long long)(VQUIT);
	e[n].n = "VSTART"; e[n++].v = (long long)(VSTART);
	e[n].n = "VSTOP"; e[n++].v = (long long)(VSTOP);
	e[n].n = "VSUSP"; e[n++].v = (long long)(VSUSP);
	e[n].n = "VTIME"; e[n++].v = (long long)(VTIME);
	if (n < 11)
		pc_unresolved("fewer than 11 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			if (is_allowed(e[i].n, e[j].n))
				continue;
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	for (i = 0; i < n; i++) {
		pc_check(e[i].v >= 0, "%s is negative", e[i].n);
#ifdef NCCS
		pc_check(e[i].v < (long long)(NCCS), "%s (%lld) is not below NCCS (%lld)", e[i].n, e[i].v, (long long)(NCCS));
#endif
	}
	pc_pass();
	}
#endif
}
