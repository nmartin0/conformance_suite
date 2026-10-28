/*
 * The date utility writes the weekday and month names of the locale named
 * in LC_ALL: its %A and %B output for the current day equals the entries
 * of the locale's lists selected by the numbers that %w and %m give in
 * the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.5-lc-time.005
 */
#include "pc.h"
#include "tlib.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_MESSAGES

static int run(const char *loc, const char *fmt, char *out, size_t n)
{
	char cmd[8400];
	FILE *p;
	size_t l;

	(void)snprintf(cmd, sizeof cmd, "LC_ALL='%s' date -u '%s' 2>/dev/null", loc, fmt);
	p = popen(cmd, "r");
	if (p == NULL)
		return -1;
	if (fgets(out, (int)n, p) == NULL)
		out[0] = '\0';
	(void)pclose(p);
	l = strlen(out);
	if (l > 0 && out[l - 1] == '\n')
		out[l - 1] = '\0';
	return 0;
}

int main(void)
{
	static char text[20000];
	char loc[4200], got[200], num1[100], num2[100], exp[200];
	size_t len;
	int w, m, k, ok = 0;

	pc_begin("XBD.7.3.5-lc-time.005");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	len = tl_start(text, sizeof text);
	len = tl_formats(text, sizeof text, len);
	len = tl_end(text, sizeof text, len);
	(void)snprintf(text + len, sizeof text - len, "%s", REST);
	ld_ok("dateloc", "cmap", text, loc, sizeof loc);
	pc_test();
	for (k = 0; k < 3 && !ok; k++) {
		if (run("C", "+%w %m", num1, sizeof num1) != 0 || run(loc, "+%A|%B|%a|%b", got, sizeof got) != 0 ||
		    run("C", "+%w %m", num2, sizeof num2) != 0)
			pc_unresolved("could not run the date utility");
		if (strcmp(num1, num2) != 0)
			continue; /* the day changed while measuring: measure again */
		if (sscanf(num1, "%d %d", &w, &m) != 2 || w < 0 || w > 6 || m < 1 || m > 12)
			pc_unresolved("the date utility's numeric output \"%s\" is not understood", num1);
		(void)snprintf(exp, sizeof exp, "%s|%s|%s|%s", tl_day[w], tl_mon[m - 1], tl_abday[w], tl_abmon[m - 1]);
		pc_check(strcmp(got, exp) == 0, "date wrote \"%s\" with the locale in LC_ALL, not \"%s\"", got, exp);
		ok = 1;
	}
	if (!ok)
		pc_unresolved("the date changed during every attempt");
	pc_pass();
}
