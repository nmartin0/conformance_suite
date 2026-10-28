/*
 * An era description given to localedef, with the alternative date and
 * time formats and alternative digits, is what the E conversions and the
 * ERA, ERA_D_FMT, ERA_T_FMT, ERA_D_T_FMT and ALT_DIGITS items give: %EC
 * writes the era name, %Ey the year offset within the era, %Ex, %EX and
 * %Ec write the alternative formats, and nl_langinfo() returns the era
 * description as given.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.009
 */
#include "pc.h"
#include "tlib.h"
#include <langinfo.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

int main(void)
{
	static char text[20000];
	char loc[4200], buf[100];
	const char *era[1] = { "+:1:2001/01/01:+*:EraZ:Z" };
	const char *alt[3] = { "zerox", "onex", "twox" };
	size_t len;
	struct tm t;
	const char *s;

	pc_begin("XBD.7.3.5-lc-time.009");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_formats(text, sizeof text, len);
	len = tl_list(text, sizeof text, len, "era", era, 1);
	len = tl_one(text, sizeof text, len, "era_d_fmt", "E%d|%m");
	len = tl_one(text, sizeof text, len, "era_t_fmt", "T%H");
	len = tl_one(text, sizeof text, len, "era_d_t_fmt", "C%d");
	len = tl_list(text, sizeof text, len, "alt_digits", alt, 3);
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("era", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	t = tl_tm(3, 2);
	t.tm_year = 105; /* 2005: year 5 of the era */
	pc_check(strftime(buf, sizeof buf, "%EC", &t) > 0 && strcmp(buf, "EraZ") == 0, "%%EC gave \"%s\", not \"EraZ\"", buf);
	pc_check(strftime(buf, sizeof buf, "%Ey", &t) > 0 && (strcmp(buf, "5") == 0 || strcmp(buf, "05") == 0), "%%Ey gave \"%s\", not 5 or 05", buf);
	pc_check(strftime(buf, sizeof buf, "%Ex", &t) > 0 && strcmp(buf, "E15|03") == 0, "%%Ex gave \"%s\", not \"E15|03\"", buf);
	pc_check(strftime(buf, sizeof buf, "%EX", &t) > 0 && strcmp(buf, "T14") == 0, "%%EX gave \"%s\", not \"T14\"", buf);
	pc_check(strftime(buf, sizeof buf, "%Ec", &t) > 0 && strcmp(buf, "C15") == 0, "%%Ec gave \"%s\", not \"C15\"", buf);
	s = nl_langinfo(ERA);
	pc_check(s != NULL && strcmp(s, "+:1:2001/01/01:+*:EraZ:Z") == 0, "ERA is \"%s\", not the era description as given", s ? s : "(null)");
	s = nl_langinfo(ERA_D_FMT);
	pc_check(s != NULL && strcmp(s, "E%d|%m") == 0, "ERA_D_FMT is \"%s\"", s ? s : "(null)");
	s = nl_langinfo(ERA_T_FMT);
	pc_check(s != NULL && strcmp(s, "T%H") == 0, "ERA_T_FMT is \"%s\"", s ? s : "(null)");
	s = nl_langinfo(ERA_D_T_FMT);
	pc_check(s != NULL && strcmp(s, "C%d") == 0, "ERA_D_T_FMT is \"%s\"", s ? s : "(null)");
	s = nl_langinfo(ALT_DIGITS);
	pc_check(s != NULL && strcmp(s, "zerox;onex;twox") == 0, "ALT_DIGITS is \"%s\", not the three symbols separated by semicolons", s ? s : "(null)");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
