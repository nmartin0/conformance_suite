/*
 * %s reads a decimal number and %z an offset of the form +hhmm or -hhmm (effect on tm unspecified).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.015
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.015");
	pc_test();
	sp_pos("86400,", "%s,", 6);
	sp_pos("0,", "%s,", 2);
	sp_pos("1700000000", "%s", 10);
	sp_pos("+0530,", "%z,", 6);
	sp_pos("-0430,", "%z,", 6);
	sp_pos("+0000", "%z", 5);
	sp_pos("2024-02-29 12:00:00 -0430", "%F %T %z", 25);
	sp_null("abc", "%s");
	sp_null("", "%s");
	sp_null("x", "%z");
	pc_pass();
}
