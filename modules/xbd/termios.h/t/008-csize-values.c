/*
 * CS5, CS6, CS7 and CS8 lie inside CSIZE and have four different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.008
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.008");
#if !defined(CSIZE) || !defined(CS5) || !defined(CS6) || !defined(CS7) || !defined(CS8)
	pc_unresolved("CSIZE or a CS constant is missing (the header check reports it)");
#else
	static const char *const nm[] = { "CS5", "CS6", "CS7", "CS8" };
	static const unsigned long long v[] = { (unsigned long long)(CS5), (unsigned long long)(CS6), (unsigned long long)(CS7), (unsigned long long)(CS8) };
	unsigned long long mask = (unsigned long long)(CSIZE);
	int i, j;

	pc_test();
	for (i = 0; i < 4; i++) {
		pc_check((v[i] & mask) == v[i], "%s (%#llx) has bits outside CSIZE (%#llx)", nm[i], v[i], mask);
		for (j = i + 1; j < 4; j++)
			pc_check(v[i] != v[j], "%s and %s are both %#llx", nm[i], nm[j], v[i]);
	}
	pc_pass();
#endif
}
