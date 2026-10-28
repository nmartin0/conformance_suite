/*
 * printf-family output uses the radix of the locale, and the %' flag
 * groups the integer digits with the locale's separator in groups of 3
 * and then of 2 (grouping 3;2).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.4-lc-numeric.002
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200], buf[100];

	pc_begin("XBD.7.3.4-lc-numeric.002");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("numf", "cmap",
	    "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3;2\nEND LC_NUMERIC\n" REST, loc,
	    sizeof loc);
	pc_test();
	ld_load(loc);
	(void)snprintf(buf, sizeof buf, "%.2f", 1.5);
	pc_check(strcmp(buf, "1,50") == 0, "%%.2f of 1.5 gave \"%s\", not \"1,50\"", buf);
	(void)snprintf(buf, sizeof buf, "%'d", 123456789);
	pc_check(strcmp(buf, "12.34.56.789") == 0, "%%'d of 123456789 gave \"%s\", not \"12.34.56.789\"", buf);
	(void)snprintf(buf, sizeof buf, "%'d", 999);
	pc_check(strcmp(buf, "999") == 0, "%%'d of 999 gave \"%s\"", buf);
	(void)snprintf(buf, sizeof buf, "%'.2f", 12345.5);
	pc_check(strcmp(buf, "12.345,50") == 0, "%%'.2f of 12345.5 gave \"%s\", not \"12.345,50\"", buf);
	(void)snprintf(buf, sizeof buf, "%d", 123456789);
	pc_check(strcmp(buf, "123456789") == 0, "%%d of 123456789 gave \"%s\": no grouping is due without the flag", buf);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
