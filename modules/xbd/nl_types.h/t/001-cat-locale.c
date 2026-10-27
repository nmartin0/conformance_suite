/*
 * NL_CAT_LOCALE is an int value different from 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.nl_types.h.001
 */
#include <nl_types.h>
#include "pc.h"


#ifdef NL_CAT_LOCALE
static const int value = NL_CAT_LOCALE;
#endif

int main(void)
{
	pc_begin("XBD.nl_types.h.001");
	pc_test();
#ifndef NL_CAT_LOCALE
	pc_fail("<nl_types.h> does not define NL_CAT_LOCALE");
#else
	pc_check(value != 0, "NL_CAT_LOCALE is 0, the same as the LANG request");
	pc_pass();
#endif
}
