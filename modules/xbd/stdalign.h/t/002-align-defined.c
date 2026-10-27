/*
 * __alignas_is_defined and __alignof_is_defined.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdalign.h.002
 */
#include <stdalign.h>
#include "pc.h"

#if __alignas_is_defined != 1
#error "__alignas_is_defined is not 1 in #if"
#endif
#if __alignof_is_defined != 1
#error "__alignof_is_defined is not 1 in #if"
#endif
_Static_assert(_Generic(__alignas_is_defined, int: 1, default: 0) && _Generic(__alignof_is_defined, int: 1, default: 0), "an _is_defined macro is not an int constant");

int main(void)
{
	pc_begin("XBD.stdalign.h.002");
	pc_test();
	pc_pass();
}
