/*
 * When both meridiem strings are empty, nl_langinfo() gives the empty
 * string for AM_STR and PM_STR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.008
 */
#include "pc.h"
#include "tlib.h"
#include <langinfo.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

int main(void)
{
	static char text[20000];
	char loc[4200];
	const char *ampm[2] = { "", "" };
	size_t len;
	const char *a, *p;

	pc_begin("XBD.7.3.5-lc-time.008");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_one(text, sizeof text, len, "d_t_fmt", "%d.%m.%y %H:%M:%S");
	len = tl_one(text, sizeof text, len, "d_fmt", "%d.%m.%y");
	len = tl_one(text, sizeof text, len, "t_fmt", "%H:%M:%S");
	len = tl_list(text, sizeof text, len, "am_pm", ampm, 2);
	len = tl_one(text, sizeof text, len, "t_fmt_ampm", "");
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("noampm", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	a = nl_langinfo(AM_STR);
	p = nl_langinfo(PM_STR);
	pc_check(a != NULL && a[0] == '\0', "AM_STR is not empty");
	pc_check(p != NULL && p[0] == '\0', "PM_STR is not empty");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
