/*
 * alignas and alignof expand as required and work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdalign.h.001
 */
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pc.h"

#define STR_(x) #x
#define STR(x) STR_(x)

_Static_assert(alignof(char) == 1, "alignof(char) is not 1");
_Static_assert(alignof(double) > 0 && (alignof(double) & (alignof(double) - 1)) == 0, "alignof(double) is not a power of two");
_Static_assert(alignof(long double) >= alignof(double), "alignof(long double) is below alignof(double)");

static alignas(alignof(max_align_t)) char bufmax[5];
static alignas(alignof(long double)) char bufld[3];

int main(void)
{
	pc_begin("XBD.stdalign.h.001");
	pc_test();
	alignas(alignof(max_align_t)) char local[7];

	pc_check(strcmp(STR(alignas), "_Alignas") == 0, "alignas does not expand to _Alignas");
	pc_check(strcmp(STR(alignof), "_Alignof") == 0, "alignof does not expand to _Alignof");
	pc_check((uintptr_t)bufmax % alignof(max_align_t) == 0, "alignas(alignof(max_align_t)) did not align a static object");
	pc_check((uintptr_t)bufld % alignof(long double) == 0, "alignas(alignof(long double)) did not align a static object");
	pc_check((uintptr_t)local % alignof(max_align_t) == 0, "alignas(alignof(max_align_t)) did not align an automatic object");
	pc_pass();
}
