/*
 * O_ACCMODE is usable in #if and every access mode constant lies wholly
 * inside it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.013
 */
#include "pc.h"
#include <fcntl.h>

#if defined(O_ACCMODE)
#if !((O_ACCMODE) || 1)
#error "O_ACCMODE is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.fcntl.h.013");
#if !defined(O_ACCMODE) || !defined(O_RDONLY) || !defined(O_WRONLY) || !defined(O_RDWR)
	pc_unresolved("O_ACCMODE or an access mode is missing (the header check reports it)");
#else
	unsigned long long mask = (unsigned long long)(O_ACCMODE);
	unsigned long long m[5];
	static const char *const nm[5] = { "O_RDONLY", "O_WRONLY", "O_RDWR", "O_EXEC", "O_SEARCH" };
	int k, n = 3;

	m[0] = (unsigned long long)(O_RDONLY);
	m[1] = (unsigned long long)(O_WRONLY);
	m[2] = (unsigned long long)(O_RDWR);
#ifdef O_EXEC
	m[n++] = (unsigned long long)(O_EXEC);
#else
	pc_note("O_EXEC is not defined (the header check reports it)");
#endif
#ifdef O_SEARCH
	m[n++] = (unsigned long long)(O_SEARCH);
#endif
	pc_test();
	for (k = 0; k < n; k++)
		pc_check((m[k] & mask) == m[k], "access mode %s (%#llx) has bits outside O_ACCMODE (%#llx)", nm[k], m[k], mask);
	pc_pass();
#endif
}
