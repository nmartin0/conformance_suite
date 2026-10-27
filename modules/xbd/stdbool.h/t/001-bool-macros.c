/*
 * The stdbool.h macros expand as required.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdbool.h.001
 */
#include <stdbool.h>
#include <string.h>
#include "pc.h"

#define STR_(x) #x
#define STR(x) STR_(x)

_Static_assert(_Generic(true, int: 1, default: 0) && _Generic(false, int: 1, default: 0), "true or false is not an int constant");
_Static_assert(true == 1 && false == 0, "true is not 1 or false is not 0");
_Static_assert(_Generic(__bool_true_false_are_defined, int: 1, default: 0), "__bool_true_false_are_defined is not an int constant");
_Static_assert(__bool_true_false_are_defined == 1, "__bool_true_false_are_defined is not 1");

int main(void)
{
	pc_begin("XBD.stdbool.h.001");
	pc_test();
	bool b = 2;
	bool c = 0;

	pc_check(strcmp(STR(bool), "_Bool") == 0, "bool does not expand to _Bool");
	pc_check(b == true && c == false, "a bool does not take the values true and false");
	pc_check(sizeof(bool) == sizeof(_Bool), "bool is not _Bool");
	pc_pass();
}
