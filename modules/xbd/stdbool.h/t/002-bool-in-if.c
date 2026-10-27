/*
 * true, false and __bool_true_false_are_defined in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdbool.h.002
 */
#include <stdbool.h>
#include "pc.h"

#if true != 1
#error "true is not 1 in #if"
#endif
#if false != 0
#error "false is not 0 in #if"
#endif
#if !true || false
#error "true or false has the wrong truth value in #if"
#endif
#if __bool_true_false_are_defined != 1
#error "__bool_true_false_are_defined is not 1 in #if"
#endif

int main(void)
{
	pc_begin("XBD.stdbool.h.002");
	pc_test();
	pc_pass();
}
