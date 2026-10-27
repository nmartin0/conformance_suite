/*
 * A weekday alone selects the first such day from today on; the time of day is the current one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.014
 */
#include "gd.h"

static void mix(const char *s, char *out)
{
	size_t i;

	for (i = 0; s[i] != '\0'; i++)
		out[i] = (i % 2) ? (char)tolower((unsigned char)s[i]) : (char)toupper((unsigned char)s[i]);
	out[i] = '\0';
}

int main(void)
{
	static const int offs[] = { 0, 1, 3, 6 };
	size_t k;
	int form;
	int try;

	pc_begin("XSH.getdate.014");
	gd_setup("%A\n%a\n%w\n");
	pc_test();
	for (form = 0; form < 5; form++) {
		for (k = 0; k < GD_N(offs); k++) {
			for (try = 0; try < 3; try++) {
				time_t t1 = time(NULL), t2, ex;
				struct tm now = *localtime(&t1), e, r;
				int target = (now.tm_wday + offs[k]) % 7;
				char name[32];

				if (form == 0)
					snprintf(name, sizeof name, "%s", gd_wday[target]);
				else if (form == 1)
					snprintf(name, sizeof name, "%s", gd_wabbr[target]);
				else if (form == 2)
					mix(gd_wday[target], name);
				else if (form == 3)
					mix(gd_wabbr[target], name);
				else
					snprintf(name, sizeof name, "%d", target);
				r = gd_get(name);
				t2 = time(NULL);
				if (!gd_same_day(t1, t2))
					continue;
				ex = t1 + (time_t)offs[k] * 86400;
				e = *localtime(&ex);
				pc_check(r.tm_wday == target, "%s: tm_wday is %d, expected %d", name, r.tm_wday, target);
				gd_fields(&r, name, e.tm_year, e.tm_mon, e.tm_mday, -1, -1, -1);
				pc_check(gd_hms_now(&r, t1, t2), "%s: the time of day is not the current one", name);
				break;
			}
			if (try == 3)
				pc_unresolved("the date changed during every attempt");
		}
	}
	pc_pass();
}
