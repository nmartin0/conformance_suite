/*
 * nl_langinfo() gives, for the LC_TIME items, the strings given to
 * localedef: weekday and month names (including the alternative months),
 * the three formats, the meridiem strings and the 12-hour format.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.007
 */
#include "pc.h"
#include "tlib.h"
#include <langinfo.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

static void chk(nl_item it, const char *want, const char *what)
{
	const char *s = nl_langinfo(it);

	pc_check(s != NULL && strcmp(s, want) == 0, "%s is \"%s\", not \"%s\"", what, s ? s : "(null)", want);
}

int main(void)
{
	static char text[20000];
	char loc[4200];
	const char *ampm[2] = { "ante", "post" };
	size_t len;
	int i;
	static const nl_item ad[7] = { ABDAY_1, ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7 };
	static const nl_item dd[7] = { DAY_1, DAY_2, DAY_3, DAY_4, DAY_5, DAY_6, DAY_7 };
	static const nl_item am[12] = { ABMON_1, ABMON_2, ABMON_3, ABMON_4, ABMON_5, ABMON_6, ABMON_7, ABMON_8, ABMON_9,
	    ABMON_10, ABMON_11, ABMON_12 };
	static const nl_item mm[12] = { MON_1, MON_2, MON_3, MON_4, MON_5, MON_6, MON_7, MON_8, MON_9, MON_10, MON_11, MON_12 };
	static const nl_item aam[12] = { ABALTMON_1, ABALTMON_2, ABALTMON_3, ABALTMON_4, ABALTMON_5, ABALTMON_6, ABALTMON_7,
	    ABALTMON_8, ABALTMON_9, ABALTMON_10, ABALTMON_11, ABALTMON_12 };
	static const nl_item alm[12] = { ALTMON_1, ALTMON_2, ALTMON_3, ALTMON_4, ALTMON_5, ALTMON_6, ALTMON_7, ALTMON_8,
	    ALTMON_9, ALTMON_10, ALTMON_11, ALTMON_12 };

	pc_begin("XBD.7.3.5-lc-time.007");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_one(text, sizeof text, len, "d_t_fmt", "D%d.%m.%y T%H:%M:%S");
	len = tl_one(text, sizeof text, len, "d_fmt", "d=%d|%m|%y");
	len = tl_one(text, sizeof text, len, "t_fmt", "t=%H;%M;%S");
	len = tl_list(text, sizeof text, len, "am_pm", ampm, 2);
	len = tl_one(text, sizeof text, len, "t_fmt_ampm", "r=%I:%M %p");
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("li", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	for (i = 0; i < 7; i++) {
		chk(ad[i], tl_abday[i], "an ABDAY item");
		chk(dd[i], tl_day[i], "a DAY item");
	}
	for (i = 0; i < 12; i++) {
		chk(am[i], tl_abmon[i], "an ABMON item");
		chk(mm[i], tl_mon[i], "a MON item");
		chk(aam[i], tl_abalt[i], "an ABALTMON item");
		chk(alm[i], tl_alt[i], "an ALTMON item");
	}
	chk(D_T_FMT, "D%d.%m.%y T%H:%M:%S", "D_T_FMT");
	chk(D_FMT, "d=%d|%m|%y", "D_FMT");
	chk(T_FMT, "t=%H;%M;%S", "T_FMT");
	chk(AM_STR, "ante", "AM_STR");
	chk(PM_STR, "post", "PM_STR");
	chk(T_FMT_AMPM, "r=%I:%M %p", "T_FMT_AMPM");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
