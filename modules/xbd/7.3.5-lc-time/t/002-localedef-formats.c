/*
 * The date and time formats given to localedef (d_t_fmt, d_fmt, t_fmt,
 * am_pm, t_fmt_ampm) are the ones that %c, %x, %X, %p and %r write, with
 * the escape sequence for a tab in a format becoming a tab, and the
 * ante- and post-meridiem strings chosen by the hour.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.002
 */
#include "pc.h"
#include "tlib.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

int main(void)
{
	static char text[20000];
	char loc[4200], buf[100];
	const char *ampm[2] = { "ante", "post" };
	size_t len;
	struct tm t;

	pc_begin("XBD.7.3.5-lc-time.002");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_one(text, sizeof text, len, "d_t_fmt", "D%d.%m.%y T%H:%M:%S");
	len = tl_one(text, sizeof text, len, "d_fmt", "d=%d|%m|%y");
	len = tl_one(text, sizeof text, len, "t_fmt", "t=%H\t%M;%S");
	len = tl_list(text, sizeof text, len, "am_pm", ampm, 2);
	len = tl_one(text, sizeof text, len, "t_fmt_ampm", "r=%I:%M %p");
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("fmts", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	t = tl_tm(3, 2);
	pc_check(strftime(buf, sizeof buf, "%c", &t) > 0 && strcmp(buf, "D15.03.00 T14:05:09") == 0, "%%c gave \"%s\", not \"D15.03.00 T14:05:09\"", buf);
	pc_check(strftime(buf, sizeof buf, "%x", &t) > 0 && strcmp(buf, "d=15|03|00") == 0, "%%x gave \"%s\", not \"d=15|03|00\"", buf);
	pc_check(strftime(buf, sizeof buf, "%X", &t) > 0 && strcmp(buf, "t=14\t05;09") == 0, "%%X gave \"%s\", not t=14, a tab and 05;09", buf);
	pc_check(strftime(buf, sizeof buf, "%p", &t) > 0 && strcmp(buf, "post") == 0, "%%p at 14 hours gave \"%s\", not \"post\"", buf);
	pc_check(strftime(buf, sizeof buf, "%r", &t) > 0 && strcmp(buf, "r=02:05 post") == 0, "%%r at 14:05 gave \"%s\", not \"r=02:05 post\"", buf);
	t.tm_hour = 3;
	pc_check(strftime(buf, sizeof buf, "%p", &t) > 0 && strcmp(buf, "ante") == 0, "%%p at 3 hours gave \"%s\", not \"ante\"", buf);
	pc_check(strftime(buf, sizeof buf, "%r", &t) > 0 && strcmp(buf, "r=03:05 ante") == 0, "%%r at 03:05 gave \"%s\", not \"r=03:05 ante\"", buf);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
