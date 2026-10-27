/*
 * NL_SETD can be used as the int set number, and (nl_catd)-1 can be
 *   formed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.nl_types.h.002
 */
#include <nl_types.h>
#include "pc.h"


#ifdef NL_SETD
static const int set_id = NL_SETD;
#endif

int main(void)
{
	nl_catd bad = (nl_catd)-1;

	pc_begin("XBD.nl_types.h.002");
	pc_test();
#ifndef NL_SETD
	pc_fail("<nl_types.h> does not define NL_SETD");
#else
	pc_check(set_id == NL_SETD, "NL_SETD changes when stored in an int");
	pc_check(bad == (nl_catd)-1, "(nl_catd)-1 does not compare equal to itself");
	pc_pass();
#endif
}
