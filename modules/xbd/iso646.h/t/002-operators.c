/*
 * The alternative spellings behave as their operators.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.iso646.h.002
 */
#include <iso646.h>
#include "pc.h"

#if (1 and 0) || !(1 and 1) || !(0 or 1) || (0 or 0) || (1 xor 1) || (not 1)
#error "and, or, xor or not misbehave in #if"
#endif

int main(void)
{
	pc_begin("XBD.iso646.h.002");
	pc_test();
	int a, b;
	unsigned x, y;

	for (a = 0; a <= 1; a++)
		for (b = 0; b <= 1; b++) {
			pc_check((a and b) == (a && b), "and differs from && for %d, %d", a, b);
			pc_check((a or b) == (a || b), "or differs from || for %d, %d", a, b);
			pc_check((not a) == (!a), "not differs from ! for %d", a);
			pc_check((a not_eq b) == (a != b), "not_eq differs from != for %d, %d", a, b);
		}
	x = 0xf0f0u;
	y = 0x0ff0u;
	pc_check((x bitand y) == (x & y), "bitand differs from &");
	pc_check((x bitor y) == (x | y), "bitor differs from |");
	pc_check((x xor y) == (x ^ y), "xor differs from ^");
	pc_check((compl x) == (~x), "compl differs from ~");
	a = x; a and_eq y; pc_check((unsigned)a == (x & y), "and_eq differs from &=");
	a = x; a or_eq y; pc_check((unsigned)a == (x | y), "or_eq differs from |=");
	a = x; a xor_eq y; pc_check((unsigned)a == (x ^ y), "xor_eq differs from ^=");
	pc_pass();
}
