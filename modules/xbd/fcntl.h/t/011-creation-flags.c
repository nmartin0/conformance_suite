/*
 * O_CLOEXEC, O_CLOFORK, O_CREAT, O_DIRECTORY, O_EXCL, O_NOCTTY,
 * O_NOFOLLOW, O_TRUNC and O_TTY_INIT are usable in #if and share no bits;
 * each is nonzero except that O_TTY_INIT may be zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.011
 */
#include "pc.h"
#include <string.h>
#include <fcntl.h>

#ifdef O_CLOEXEC
#if !((O_CLOEXEC) || 1)
#error "O_CLOEXEC is not usable in #if"
#endif
#endif
#ifdef O_CREAT
#if !((O_CREAT) || 1)
#error "O_CREAT is not usable in #if"
#endif
#endif
#ifdef O_DIRECTORY
#if !((O_DIRECTORY) || 1)
#error "O_DIRECTORY is not usable in #if"
#endif
#endif
#ifdef O_EXCL
#if !((O_EXCL) || 1)
#error "O_EXCL is not usable in #if"
#endif
#endif
#ifdef O_NOCTTY
#if !((O_NOCTTY) || 1)
#error "O_NOCTTY is not usable in #if"
#endif
#endif
#ifdef O_NOFOLLOW
#if !((O_NOFOLLOW) || 1)
#error "O_NOFOLLOW is not usable in #if"
#endif
#endif
#ifdef O_TRUNC
#if !((O_TRUNC) || 1)
#error "O_TRUNC is not usable in #if"
#endif
#endif
#ifdef O_TTY_INIT
#if !((O_TTY_INIT) || 1)
#error "O_TTY_INIT is not usable in #if"
#endif
#endif
#ifdef O_CLOFORK
#if !((O_CLOFORK) || 1)
#error "O_CLOFORK is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.fcntl.h.011");
#if !defined(O_CLOEXEC) || !defined(O_CREAT) || !defined(O_DIRECTORY) || !defined(O_EXCL) || !defined(O_NOCTTY) || !defined(O_NOFOLLOW) || !defined(O_TRUNC) || !defined(O_TTY_INIT)
	pc_unresolved("a constant is missing (the header check reports it): O_CLOEXEC O_CREAT O_DIRECTORY O_EXCL O_NOCTTY O_NOFOLLOW O_TRUNC O_TTY_INIT");
#else
	{
	struct { const char *n; long long v; } e[10];
	int n = 0, i, j;

	e[n].n = "O_CLOEXEC"; e[n++].v = (long long)(O_CLOEXEC);
	e[n].n = "O_CREAT"; e[n++].v = (long long)(O_CREAT);
	e[n].n = "O_DIRECTORY"; e[n++].v = (long long)(O_DIRECTORY);
	e[n].n = "O_EXCL"; e[n++].v = (long long)(O_EXCL);
	e[n].n = "O_NOCTTY"; e[n++].v = (long long)(O_NOCTTY);
	e[n].n = "O_NOFOLLOW"; e[n++].v = (long long)(O_NOFOLLOW);
	e[n].n = "O_TRUNC"; e[n++].v = (long long)(O_TRUNC);
	e[n].n = "O_TTY_INIT"; e[n++].v = (long long)(O_TTY_INIT);
#ifdef O_CLOFORK
	e[n].n = "O_CLOFORK"; e[n++].v = (long long)(O_CLOFORK);
#endif
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0 || !strcmp(e[i].n, "O_TTY_INIT"), "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(((unsigned long long)e[i].v & (unsigned long long)e[j].v) == 0, "%s (%#llx) and %s (%#llx) have bits in common", e[i].n, (unsigned long long)e[i].v, e[j].n, (unsigned long long)e[j].v);
		}
	pc_pass();
	}
#endif
}
