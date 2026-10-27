/*
 * NCCS is positive and c_cc has exactly NCCS elements.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.002
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	struct termios t;

	pc_begin("XBD.termios.h.002");
#ifndef NCCS
	pc_unresolved("NCCS is missing (the header check reports it)");
#else
	pc_test();
	pc_check((NCCS) > 0, "NCCS is not positive");
	pc_check(sizeof t.c_cc / sizeof t.c_cc[0] == (size_t)(NCCS), "c_cc does not have NCCS elements");
	pc_pass();
#endif
}
