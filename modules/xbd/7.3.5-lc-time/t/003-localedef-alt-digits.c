/*
 * The alt_digits list given to localedef supplies the symbols written for
 * the numeric conversion specifications under the %O modifier: entry n
 * stands for the value n, while the unmodified conversions still write
 * ordinary digits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.003
 */
#include "pc.h"
#include "tlib.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES
#define NALT 32

int main(void)
{
	static char text[20000], names[NALT][4];
	const char *alt[NALT];
	char loc[4200], buf[100];
	size_t len;
	struct tm t;
	int i;

	pc_begin("XBD.7.3.5-lc-time.003");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	for (i = 0; i < NALT; i++) {
		names[i][0] = 'q';
		names[i][1] = (char)('a' + i / 10);
		names[i][2] = (char)('a' + i % 10);
		names[i][3] = '\0';
		alt[i] = names[i];
	}
	len = tl_start(text, sizeof text);
	len = tl_formats(text, sizeof text, len);
	len = tl_list(text, sizeof text, len, "alt_digits", alt, NALT);
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("altd", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	t = tl_tm(3, 2);
	pc_check(strftime(buf, sizeof buf, "%Od", &t) > 0 && strcmp(buf, names[15]) == 0, "%%Od for day 15 gave \"%s\", not \"%s\"", buf, names[15]);
	pc_check(strftime(buf, sizeof buf, "%OH", &t) > 0 && strcmp(buf, names[14]) == 0, "%%OH for hour 14 gave \"%s\", not \"%s\"", buf, names[14]);
	pc_check(strftime(buf, sizeof buf, "%OM", &t) > 0 && strcmp(buf, names[5]) == 0, "%%OM for minute 5 gave \"%s\", not \"%s\"", buf, names[5]);
	pc_check(strftime(buf, sizeof buf, "%OS", &t) > 0 && strcmp(buf, names[9]) == 0, "%%OS for second 9 gave \"%s\", not \"%s\"", buf, names[9]);
	pc_check(strftime(buf, sizeof buf, "%Om", &t) > 0 && strcmp(buf, names[3]) == 0, "%%Om for month 3 gave \"%s\", not \"%s\"", buf, names[3]);
	pc_check(strftime(buf, sizeof buf, "%Oy", &t) > 0 && strcmp(buf, names[0]) == 0, "%%Oy for year 00 gave \"%s\", not \"%s\"", buf, names[0]);
	pc_check(strftime(buf, sizeof buf, "%d", &t) > 0 && strcmp(buf, "15") == 0, "%%d gave \"%s\" instead of ordinary digits", buf);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
