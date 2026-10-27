/*
 * On XSI systems NL0 and NL1, CR0 to CR3, TAB0 to TAB3, BS0 and BS1, VT0
 * and VT1, and FF0 and FF1 each lie inside their mask (NLDLY, CRDLY,
 * TABDLY, BSDLY, VTDLY, FFDLY) and differ from each other within a group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.006
 */
#include "pc.h"
#include <termios.h>

/* The values a delay field takes lie inside its mask and differ from each other. */
static void field(const char *mn, unsigned long long mask, const char *const *nm, const unsigned long long *v, int n)
{
	int i, j;

	for (i = 0; i < n; i++) {
		pc_check((v[i] & mask) == v[i], "%s (%#llx) has bits outside %s (%#llx)", nm[i], v[i], mn, mask);
		for (j = i + 1; j < n; j++)
			pc_check(v[i] != v[j], "%s and %s are both %#llx", nm[i], nm[j], v[i]);
	}
}

int main(void)
{
	pc_begin("XBD.termios.h.006");
#if !defined(NLDLY) || !defined(NL0) || !defined(NL1) || !defined(CRDLY) || !defined(CR0) || !defined(CR1) || !defined(CR2) || !defined(CR3) || !defined(TABDLY) || !defined(TAB0) || !defined(TAB1) || !defined(TAB2) || !defined(TAB3) || !defined(BSDLY) || !defined(BS0) || !defined(BS1) || !defined(VTDLY) || !defined(VT0) || !defined(VT1) || !defined(FFDLY) || !defined(FF0) || !defined(FF1)
	pc_unresolved("a delay constant is missing (the header check reports it)");
#else
	static const char *const nl[] = { "NL0", "NL1" };
	static const unsigned long long nlv[] = { (unsigned long long)(NL0), (unsigned long long)(NL1) };
	static const char *const cr[] = { "CR0", "CR1", "CR2", "CR3" };
	static const unsigned long long crv[] = { (unsigned long long)(CR0), (unsigned long long)(CR1), (unsigned long long)(CR2), (unsigned long long)(CR3) };
	static const char *const tab[] = { "TAB0", "TAB1", "TAB2", "TAB3" };
	static const unsigned long long tabv[] = { (unsigned long long)(TAB0), (unsigned long long)(TAB1), (unsigned long long)(TAB2), (unsigned long long)(TAB3) };
	static const char *const bs[] = { "BS0", "BS1" };
	static const unsigned long long bsv[] = { (unsigned long long)(BS0), (unsigned long long)(BS1) };
	static const char *const vt[] = { "VT0", "VT1" };
	static const unsigned long long vtv[] = { (unsigned long long)(VT0), (unsigned long long)(VT1) };
	static const char *const ff[] = { "FF0", "FF1" };
	static const unsigned long long ffv[] = { (unsigned long long)(FF0), (unsigned long long)(FF1) };

	pc_test();
	field("NLDLY", (unsigned long long)(NLDLY), nl, nlv, 2);
	field("CRDLY", (unsigned long long)(CRDLY), cr, crv, 4);
	field("TABDLY", (unsigned long long)(TABDLY), tab, tabv, 4);
	field("BSDLY", (unsigned long long)(BSDLY), bs, bsv, 2);
	field("VTDLY", (unsigned long long)(VTDLY), vt, vtv, 2);
	field("FFDLY", (unsigned long long)(FFDLY), ff, ffv, 2);
	pc_pass();
#endif
}
