/*
 * noreturn expands to _Noreturn and declares a non-returning function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdnoreturn.h.001
 */
#include <stdnoreturn.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"

#define STR_(x) #x
#define STR(x) STR_(x)

static noreturn void leave(int c)
{
	_Exit(c);
}

noreturn void leave_decl(int);

void leave_decl(int c)
{
	_Exit(c);
}

int main(void)
{
	pc_begin("XBD.stdnoreturn.h.001");
	pc_test();
	volatile int zero = 0;

	pc_check(strcmp(STR(noreturn), "_Noreturn") == 0, "noreturn does not expand to _Noreturn");
	if (zero) {
		leave(1);
		leave_decl(1);
	}
	pc_pass();
}
