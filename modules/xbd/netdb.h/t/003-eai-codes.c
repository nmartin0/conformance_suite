/*
 * The EAI_ error values are usable in #if, non-zero and all different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.003
 */
#include <netdb.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.netdb.h.003");
	pc_test();
#if !defined(EAI_AGAIN) || !defined(EAI_BADFLAGS) || !defined(EAI_FAIL) || !defined(EAI_FAMILY) || !defined(EAI_MEMORY) || !defined(EAI_NONAME) || !defined(EAI_SERVICE) || !defined(EAI_SOCKTYPE) || !defined(EAI_SYSTEM) || !defined(EAI_OVERFLOW)
	pc_fail("<netdb.h> does not define every EAI_ error value");
#elif EAI_AGAIN == 0 || EAI_BADFLAGS == 0 || EAI_FAIL == 0 || EAI_FAMILY == 0 || EAI_MEMORY == 0 || EAI_NONAME == 0 || EAI_SERVICE == 0 || EAI_SOCKTYPE == 0 || EAI_SYSTEM == 0 || EAI_OVERFLOW == 0
	pc_fail("an EAI_ error value is 0, the success value");
#elif EAI_AGAIN == EAI_BADFLAGS || \
    EAI_AGAIN == EAI_FAIL || \
    EAI_AGAIN == EAI_FAMILY || \
    EAI_AGAIN == EAI_MEMORY || \
    EAI_AGAIN == EAI_NONAME || \
    EAI_AGAIN == EAI_SERVICE || \
    EAI_AGAIN == EAI_SOCKTYPE || \
    EAI_AGAIN == EAI_SYSTEM || \
    EAI_AGAIN == EAI_OVERFLOW || \
    EAI_BADFLAGS == EAI_FAIL || \
    EAI_BADFLAGS == EAI_FAMILY || \
    EAI_BADFLAGS == EAI_MEMORY || \
    EAI_BADFLAGS == EAI_NONAME || \
    EAI_BADFLAGS == EAI_SERVICE || \
    EAI_BADFLAGS == EAI_SOCKTYPE || \
    EAI_BADFLAGS == EAI_SYSTEM || \
    EAI_BADFLAGS == EAI_OVERFLOW || \
    EAI_FAIL == EAI_FAMILY || \
    EAI_FAIL == EAI_MEMORY || \
    EAI_FAIL == EAI_NONAME || \
    EAI_FAIL == EAI_SERVICE || \
    EAI_FAIL == EAI_SOCKTYPE || \
    EAI_FAIL == EAI_SYSTEM || \
    EAI_FAIL == EAI_OVERFLOW || \
    EAI_FAMILY == EAI_MEMORY || \
    EAI_FAMILY == EAI_NONAME || \
    EAI_FAMILY == EAI_SERVICE || \
    EAI_FAMILY == EAI_SOCKTYPE || \
    EAI_FAMILY == EAI_SYSTEM || \
    EAI_FAMILY == EAI_OVERFLOW || \
    EAI_MEMORY == EAI_NONAME || \
    EAI_MEMORY == EAI_SERVICE || \
    EAI_MEMORY == EAI_SOCKTYPE || \
    EAI_MEMORY == EAI_SYSTEM || \
    EAI_MEMORY == EAI_OVERFLOW || \
    EAI_NONAME == EAI_SERVICE || \
    EAI_NONAME == EAI_SOCKTYPE || \
    EAI_NONAME == EAI_SYSTEM || \
    EAI_NONAME == EAI_OVERFLOW || \
    EAI_SERVICE == EAI_SOCKTYPE || \
    EAI_SERVICE == EAI_SYSTEM || \
    EAI_SERVICE == EAI_OVERFLOW || \
    EAI_SOCKTYPE == EAI_SYSTEM || \
    EAI_SOCKTYPE == EAI_OVERFLOW || \
    EAI_SYSTEM == EAI_OVERFLOW
	pc_fail("two of the EAI_ error values are the same");
#else
	pc_pass();
#endif
}
