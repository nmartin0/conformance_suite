/*
 * cc_t, speed_t and tcflag_t are unsigned integer types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.001
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.001");
	pc_test();
	pc_check((cc_t)-1 > 0, "cc_t is not an unsigned type");
	pc_check((cc_t)0.5 == 0, "cc_t is not an integer type");
	pc_check((speed_t)-1 > 0, "speed_t is not an unsigned type");
	pc_check((speed_t)0.5 == 0, "speed_t is not an integer type");
	pc_check((tcflag_t)-1 > 0, "tcflag_t is not an unsigned type");
	pc_check((tcflag_t)0.5 == 0, "tcflag_t is not an integer type");
	pc_pass();
}
