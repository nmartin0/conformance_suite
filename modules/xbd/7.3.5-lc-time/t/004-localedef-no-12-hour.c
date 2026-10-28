/*
 * With both am_pm strings empty and an empty t_fmt_ampm, %p writes
 * nothing and %r writes nothing or a 24-hour time, while the 24-hour
 * formats still work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.004
 */
#include "pc.h"
#include "tlib.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

int main(void)
{
	static char text[20000];
	char loc[4200], buf[100];
	const char *ampm[2] = { "", "" };
	size_t len;
	struct tm t;

	pc_begin("XBD.7.3.5-lc-time.004");
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
	ld_ok("no12", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	t = tl_tm(3, 2);
	pc_check(strftime(buf, sizeof buf, "<%p>", &t) == 2 && strcmp(buf, "<>") == 0, "%%p wrote \"%s\" although both meridiem strings are empty", buf);
	t.tm_hour = 3;
	pc_check(strftime(buf, sizeof buf, "<%p>", &t) == 2 && strcmp(buf, "<>") == 0, "%%p wrote \"%s\" in the morning although both meridiem strings are empty", buf);
	/* %r is then either the empty string or a 24-hour notation of the time (03, 05 and 09 in order) */
	pc_check(strftime(buf, sizeof buf, "<%r>", &t) > 0 &&
	    (strcmp(buf, "<>") == 0 ||
	    (strstr(buf, "03") != NULL && strstr(strstr(buf, "03") + 2, "05") != NULL &&
	    strstr(strstr(strstr(buf, "03") + 2, "05") + 2, "09") != NULL)),
	    "%%r wrote \"%s\", neither empty nor a 24-hour time, although the 12-hour format is not supported", buf);
	pc_check(strftime(buf, sizeof buf, "%X", &t) > 0 && strcmp(buf, "03:05:09") == 0, "%%X gave \"%s\", not \"03:05:09\"", buf);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
