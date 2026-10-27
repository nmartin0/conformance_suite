/*
 * S_IFBLK, S_IFCHR, S_IFIFO, S_IFREG, S_IFDIR, S_IFLNK and S_IFSOCK are
 * usable in #if and have seven different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.004
 */
#include "pc.h"
#include <sys/stat.h>

#ifdef S_IFBLK
#if !((S_IFBLK) || 1)
#error "S_IFBLK is not usable in #if"
#endif
#endif
#ifdef S_IFCHR
#if !((S_IFCHR) || 1)
#error "S_IFCHR is not usable in #if"
#endif
#endif
#ifdef S_IFIFO
#if !((S_IFIFO) || 1)
#error "S_IFIFO is not usable in #if"
#endif
#endif
#ifdef S_IFREG
#if !((S_IFREG) || 1)
#error "S_IFREG is not usable in #if"
#endif
#endif
#ifdef S_IFDIR
#if !((S_IFDIR) || 1)
#error "S_IFDIR is not usable in #if"
#endif
#endif
#ifdef S_IFLNK
#if !((S_IFLNK) || 1)
#error "S_IFLNK is not usable in #if"
#endif
#endif
#ifdef S_IFSOCK
#if !((S_IFSOCK) || 1)
#error "S_IFSOCK is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.sys_stat.h.004");
#if !defined(S_IFBLK) || !defined(S_IFCHR) || !defined(S_IFIFO) || !defined(S_IFREG) || !defined(S_IFDIR) || !defined(S_IFLNK) || !defined(S_IFSOCK)
	pc_unresolved("a constant is missing (the header check reports it): S_IFBLK S_IFCHR S_IFIFO S_IFREG S_IFDIR S_IFLNK S_IFSOCK");
#else
	{
	struct { const char *n; long long v; } e[8];
	int n = 0, i, j;

	e[n].n = "S_IFBLK"; e[n++].v = (long long)(S_IFBLK);
	e[n].n = "S_IFCHR"; e[n++].v = (long long)(S_IFCHR);
	e[n].n = "S_IFIFO"; e[n++].v = (long long)(S_IFIFO);
	e[n].n = "S_IFREG"; e[n++].v = (long long)(S_IFREG);
	e[n].n = "S_IFDIR"; e[n++].v = (long long)(S_IFDIR);
	e[n].n = "S_IFLNK"; e[n++].v = (long long)(S_IFLNK);
	e[n].n = "S_IFSOCK"; e[n++].v = (long long)(S_IFSOCK);
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
