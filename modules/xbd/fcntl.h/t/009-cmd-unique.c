/*
 * The fcntl() command constants are usable in #if and have different
 * values (F_DUPFD, F_DUPFD_CLOEXEC, F_DUPFD_CLOFORK, F_GETFD, F_SETFD,
 * F_GETFL, F_SETFL, F_GETLK, F_SETLK, F_SETLKW, the F_OFD_* three,
 * F_GETOWN, F_SETOWN, F_GETOWN_EX, F_SETOWN_EX).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.009
 */
#include "pc.h"
#include <fcntl.h>

#ifdef F_DUPFD
#if !((F_DUPFD) || 1)
#error "F_DUPFD is not usable in #if"
#endif
#endif
#ifdef F_DUPFD_CLOEXEC
#if !((F_DUPFD_CLOEXEC) || 1)
#error "F_DUPFD_CLOEXEC is not usable in #if"
#endif
#endif
#ifdef F_GETFD
#if !((F_GETFD) || 1)
#error "F_GETFD is not usable in #if"
#endif
#endif
#ifdef F_SETFD
#if !((F_SETFD) || 1)
#error "F_SETFD is not usable in #if"
#endif
#endif
#ifdef F_GETFL
#if !((F_GETFL) || 1)
#error "F_GETFL is not usable in #if"
#endif
#endif
#ifdef F_SETFL
#if !((F_SETFL) || 1)
#error "F_SETFL is not usable in #if"
#endif
#endif
#ifdef F_GETLK
#if !((F_GETLK) || 1)
#error "F_GETLK is not usable in #if"
#endif
#endif
#ifdef F_SETLK
#if !((F_SETLK) || 1)
#error "F_SETLK is not usable in #if"
#endif
#endif
#ifdef F_SETLKW
#if !((F_SETLKW) || 1)
#error "F_SETLKW is not usable in #if"
#endif
#endif
#ifdef F_GETOWN
#if !((F_GETOWN) || 1)
#error "F_GETOWN is not usable in #if"
#endif
#endif
#ifdef F_SETOWN
#if !((F_SETOWN) || 1)
#error "F_SETOWN is not usable in #if"
#endif
#endif
#ifdef F_DUPFD_CLOFORK
#if !((F_DUPFD_CLOFORK) || 1)
#error "F_DUPFD_CLOFORK is not usable in #if"
#endif
#endif
#ifdef F_OFD_GETLK
#if !((F_OFD_GETLK) || 1)
#error "F_OFD_GETLK is not usable in #if"
#endif
#endif
#ifdef F_OFD_SETLK
#if !((F_OFD_SETLK) || 1)
#error "F_OFD_SETLK is not usable in #if"
#endif
#endif
#ifdef F_OFD_SETLKW
#if !((F_OFD_SETLKW) || 1)
#error "F_OFD_SETLKW is not usable in #if"
#endif
#endif
#ifdef F_GETOWN_EX
#if !((F_GETOWN_EX) || 1)
#error "F_GETOWN_EX is not usable in #if"
#endif
#endif
#ifdef F_SETOWN_EX
#if !((F_SETOWN_EX) || 1)
#error "F_SETOWN_EX is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.fcntl.h.009");
#if !defined(F_DUPFD) || !defined(F_DUPFD_CLOEXEC) || !defined(F_GETFD) || !defined(F_SETFD) || !defined(F_GETFL) || !defined(F_SETFL) || !defined(F_GETLK) || !defined(F_SETLK) || !defined(F_SETLKW) || !defined(F_GETOWN) || !defined(F_SETOWN)
	pc_unresolved("a constant is missing (the header check reports it): F_DUPFD F_DUPFD_CLOEXEC F_GETFD F_SETFD F_GETFL F_SETFL F_GETLK F_SETLK F_SETLKW F_GETOWN F_SETOWN");
#else
	{
	struct { const char *n; long long v; } e[18];
	int n = 0, i, j;

	e[n].n = "F_DUPFD"; e[n++].v = (long long)(F_DUPFD);
	e[n].n = "F_DUPFD_CLOEXEC"; e[n++].v = (long long)(F_DUPFD_CLOEXEC);
	e[n].n = "F_GETFD"; e[n++].v = (long long)(F_GETFD);
	e[n].n = "F_SETFD"; e[n++].v = (long long)(F_SETFD);
	e[n].n = "F_GETFL"; e[n++].v = (long long)(F_GETFL);
	e[n].n = "F_SETFL"; e[n++].v = (long long)(F_SETFL);
	e[n].n = "F_GETLK"; e[n++].v = (long long)(F_GETLK);
	e[n].n = "F_SETLK"; e[n++].v = (long long)(F_SETLK);
	e[n].n = "F_SETLKW"; e[n++].v = (long long)(F_SETLKW);
	e[n].n = "F_GETOWN"; e[n++].v = (long long)(F_GETOWN);
	e[n].n = "F_SETOWN"; e[n++].v = (long long)(F_SETOWN);
#ifdef F_DUPFD_CLOFORK
	e[n].n = "F_DUPFD_CLOFORK"; e[n++].v = (long long)(F_DUPFD_CLOFORK);
#endif
#ifdef F_OFD_GETLK
	e[n].n = "F_OFD_GETLK"; e[n++].v = (long long)(F_OFD_GETLK);
#endif
#ifdef F_OFD_SETLK
	e[n].n = "F_OFD_SETLK"; e[n++].v = (long long)(F_OFD_SETLK);
#endif
#ifdef F_OFD_SETLKW
	e[n].n = "F_OFD_SETLKW"; e[n++].v = (long long)(F_OFD_SETLKW);
#endif
#ifdef F_GETOWN_EX
	e[n].n = "F_GETOWN_EX"; e[n++].v = (long long)(F_GETOWN_EX);
#endif
#ifdef F_SETOWN_EX
	e[n].n = "F_SETOWN_EX"; e[n++].v = (long long)(F_SETOWN_EX);
#endif
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
