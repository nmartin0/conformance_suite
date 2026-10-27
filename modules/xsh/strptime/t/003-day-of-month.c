/*
 * %d and %e read a day 1 to 31 with or without a leading zero; other values fail.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.003
 */
#include "sp.h"

int main(void)
{
	static const char *const fmts[] = { "%d,", "%e,", "%Od,", "%Oe," };
	size_t f;

	pc_begin("XSH.strptime.003");
	pc_test();
	for (f = 0; f < sizeof fmts / sizeof fmts[0]; f++) {
		sp_case("5,", fmts[f], 2, SP_MDAY, (struct tm){ .tm_mday = 5 });
		sp_case("05,", fmts[f], 3, SP_MDAY, (struct tm){ .tm_mday = 5 });
		sp_case("1,", fmts[f], 2, SP_MDAY, (struct tm){ .tm_mday = 1 });
		sp_case("17,", fmts[f], 3, SP_MDAY, (struct tm){ .tm_mday = 17 });
		sp_case("31,", fmts[f], 3, SP_MDAY, (struct tm){ .tm_mday = 31 });
		sp_case("31x", "%d", 2, SP_MDAY, (struct tm){ .tm_mday = 31 });
		sp_null("32,", fmts[f]);
		sp_null("0,", fmts[f]);
		sp_null("00,", fmts[f]);
		sp_null("x,", fmts[f]);
	}
	pc_pass();
}
