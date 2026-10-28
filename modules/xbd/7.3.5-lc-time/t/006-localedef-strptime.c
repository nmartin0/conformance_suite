/*
 * strptime() accepts the locale's weekday and month names for %A, %a, %B
 * and %b and stores the matching number, and the locale's meridiem strings
 * for %p.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.006
 */
#include "pc.h"
#include "tlib.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

int main(void)
{
	static char text[20000];
	char loc[4200], in[100];
	size_t len;
	struct tm t;
	char *e;

	pc_begin("XBD.7.3.5-lc-time.006");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_formats(text, sizeof text, len);
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("strp", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	memset(&t, 0, sizeof t);
	(void)snprintf(in, sizeof in, "%s!", tl_day[4]);
	e = strptime(in, "%A", &t);
	pc_check(e != NULL && *e == '!' && t.tm_wday == 4, "%%A did not read \"%s\" as weekday 4", in);
	memset(&t, 0, sizeof t);
	(void)snprintf(in, sizeof in, "%s!", tl_abday[6]);
	e = strptime(in, "%a", &t);
	pc_check(e != NULL && *e == '!' && t.tm_wday == 6, "%%a did not read \"%s\" as weekday 6", in);
	memset(&t, 0, sizeof t);
	(void)snprintf(in, sizeof in, "%s!", tl_mon[10]);
	e = strptime(in, "%B", &t);
	pc_check(e != NULL && *e == '!' && t.tm_mon == 10, "%%B did not read \"%s\" as month 10", in);
	memset(&t, 0, sizeof t);
	(void)snprintf(in, sizeof in, "%s!", tl_abmon[1]);
	e = strptime(in, "%b", &t);
	pc_check(e != NULL && *e == '!' && t.tm_mon == 1, "%%b did not read \"%s\" as month 1", in);
	memset(&t, 0, sizeof t);
	e = strptime("03 post!", "%I %p", &t);
	pc_check(e != NULL && *e == '!' && t.tm_hour == 15, "\"03 post\" with %%I %%p did not give hour 15");
	memset(&t, 0, sizeof t);
	e = strptime("03 ante!", "%I %p", &t);
	pc_check(e != NULL && *e == '!' && t.tm_hour == 3, "\"03 ante\" with %%I %%p did not give hour 3");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
