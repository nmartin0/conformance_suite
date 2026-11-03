/*
 * TEXTDOMAINMAX, if <libintl.h> defines it, equals {TEXTDOMAIN_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.libintl.h.001
 */
#include <libintl.h>
#include <limits.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.libintl.h.001");
#ifndef TEXTDOMAINMAX
	pc_notinuse("<libintl.h> does not define TEXTDOMAINMAX (it may omit it)");
#elif !defined(TEXTDOMAIN_MAX)
	pc_notinuse("<limits.h> defines no {TEXTDOMAIN_MAX} to compare with");
#else
	pc_test();
	pc_check((long long)(TEXTDOMAINMAX) == (long long)(TEXTDOMAIN_MAX),
	    "TEXTDOMAINMAX is %lld but {TEXTDOMAIN_MAX} is %lld",
	    (long long)(TEXTDOMAINMAX), (long long)(TEXTDOMAIN_MAX));
	pc_pass();
#endif
}
