/*
 * The weekday and month name lists given to localedef (abday, day, abmon,
 * mon, and the alternative month names) are the strings that the
 * conversion specifications %a, %A, %b, %B, %Ob and %OB write, in order
 * from Sunday and from January.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.001
 */
#include "pc.h"
#include "tlib.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

int main(void)
{
	static char text[20000];
	char loc[4200], buf[100];
	size_t len;
	struct tm t;
	int i;

	pc_begin("XBD.7.3.5-lc-time.001");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_formats(text, sizeof text, len);
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("names", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	for (i = 0; i < 7; i++) {
		t = tl_tm(i, 2);
		pc_check(strftime(buf, sizeof buf, "%a", &t) > 0 && strcmp(buf, tl_abday[i]) == 0,
		    "%%a for weekday %d gave \"%s\", not \"%s\"", i, buf, tl_abday[i]);
		pc_check(strftime(buf, sizeof buf, "%A", &t) > 0 && strcmp(buf, tl_day[i]) == 0,
		    "%%A for weekday %d gave \"%s\", not \"%s\"", i, buf, tl_day[i]);
	}
	for (i = 0; i < 12; i++) {
		t = tl_tm(3, i);
		pc_check(strftime(buf, sizeof buf, "%b", &t) > 0 && strcmp(buf, tl_abmon[i]) == 0,
		    "%%b for month %d gave \"%s\", not \"%s\"", i, buf, tl_abmon[i]);
		pc_check(strftime(buf, sizeof buf, "%B", &t) > 0 && strcmp(buf, tl_mon[i]) == 0,
		    "%%B for month %d gave \"%s\", not \"%s\"", i, buf, tl_mon[i]);
		pc_check(strftime(buf, sizeof buf, "%Ob", &t) > 0 && strcmp(buf, tl_abalt[i]) == 0,
		    "%%Ob for month %d gave \"%s\", not \"%s\"", i, buf, tl_abalt[i]);
		pc_check(strftime(buf, sizeof buf, "%OB", &t) > 0 && strcmp(buf, tl_alt[i]) == 0,
		    "%%OB for month %d gave \"%s\", not \"%s\"", i, buf, tl_alt[i]);
	}
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
