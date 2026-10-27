/*
 * On XSI systems S_IFMT is usable in #if, each file type constant lies
 * wholly inside it, and it shares no bit with the permission, set-user-ID
 * and set-group-ID bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.013
 */
#include "pc.h"
#include <sys/stat.h>

#if defined(S_IFMT)
#if !((S_IFMT) || 1)
#error "S_IFMT is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.sys_stat.h.013");
#if !defined(S_IFMT) || !defined(S_IFBLK) || !defined(S_IFCHR) || !defined(S_IFIFO) || !defined(S_IFREG) || !defined(S_IFDIR) || !defined(S_IFLNK) || !defined(S_IFSOCK) || !defined(S_IRWXU) || !defined(S_IRWXG) || !defined(S_IRWXO) || !defined(S_ISUID) || !defined(S_ISGID)
	pc_unresolved("S_IFMT or a constant it is related to is missing (the header check reports it)");
#else
	unsigned long long mask = (unsigned long long)(S_IFMT);
	static const char *const nm[7] = { "S_IFBLK", "S_IFCHR", "S_IFIFO", "S_IFREG", "S_IFDIR", "S_IFLNK", "S_IFSOCK" };
	unsigned long long ty[7];
	unsigned long long perm = (unsigned long long)(S_IRWXU) | (unsigned long long)(S_IRWXG) | (unsigned long long)(S_IRWXO) | (unsigned long long)(S_ISUID) | (unsigned long long)(S_ISGID);
	int k;

	ty[0] = (unsigned long long)(S_IFBLK);
	ty[1] = (unsigned long long)(S_IFCHR);
	ty[2] = (unsigned long long)(S_IFIFO);
	ty[3] = (unsigned long long)(S_IFREG);
	ty[4] = (unsigned long long)(S_IFDIR);
	ty[5] = (unsigned long long)(S_IFLNK);
	ty[6] = (unsigned long long)(S_IFSOCK);
	pc_test();
	for (k = 0; k < 7; k++)
		pc_check((ty[k] & mask) == ty[k], "%s (%#llo) has bits outside S_IFMT (%#llo)", nm[k], ty[k], mask);
	pc_check((mask & perm) == 0, "S_IFMT (%#llo) shares bits with the permission and set-ID bits (%#llo)", mask, perm);
	pc_pass();
#endif
}
