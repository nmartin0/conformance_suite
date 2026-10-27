/*
 * O_RDONLY, O_WRONLY and O_RDWR (and O_EXEC and O_SEARCH when defined) are
 * usable in #if and have different values, except that O_EXEC and O_SEARCH
 * may be equal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.012
 */
#include "pc.h"
#include <string.h>
#include <fcntl.h>

#ifdef O_RDONLY
#if !((O_RDONLY) || 1)
#error "O_RDONLY is not usable in #if"
#endif
#endif
#ifdef O_WRONLY
#if !((O_WRONLY) || 1)
#error "O_WRONLY is not usable in #if"
#endif
#endif
#ifdef O_RDWR
#if !((O_RDWR) || 1)
#error "O_RDWR is not usable in #if"
#endif
#endif
#ifdef O_EXEC
#if !((O_EXEC) || 1)
#error "O_EXEC is not usable in #if"
#endif
#endif
#ifdef O_SEARCH
#if !((O_SEARCH) || 1)
#error "O_SEARCH is not usable in #if"
#endif
#endif

static const char *const allowed[][2] = {
	{ "O_EXEC", "O_SEARCH" },
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
	pc_begin("XBD.fcntl.h.012");
#if !defined(O_RDONLY) || !defined(O_WRONLY) || !defined(O_RDWR)
	pc_unresolved("a constant is missing (the header check reports it): O_RDONLY O_WRONLY O_RDWR");
#else
	{
	struct { const char *n; long long v; } e[6];
	int n = 0, i, j;

	e[n].n = "O_RDONLY"; e[n++].v = (long long)(O_RDONLY);
	e[n].n = "O_WRONLY"; e[n++].v = (long long)(O_WRONLY);
	e[n].n = "O_RDWR"; e[n++].v = (long long)(O_RDWR);
#ifdef O_EXEC
	e[n].n = "O_EXEC"; e[n++].v = (long long)(O_EXEC);
#endif
#ifdef O_SEARCH
	e[n].n = "O_SEARCH"; e[n++].v = (long long)(O_SEARCH);
#endif
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			if (is_allowed(e[i].n, e[j].n))
				continue;
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
