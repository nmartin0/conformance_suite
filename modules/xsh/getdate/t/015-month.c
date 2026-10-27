/*
 * A month alone selects the first such month from the current one on, with day 1 and the current time of day.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.015
 */
#include "gd.h"

int main(void)
{
	static const int offs[] = { 0, 1, 3, 9, 11 };
	size_t k;
	int form;
	int try;

	pc_begin("XSH.getdate.015");
	gd_setup("%B\n%b\n");
	pc_test();
	for (form = 0; form < 2; form++) {
		for (k = 0; k < GD_N(offs); k++) {
			for (try = 0; try < 3; try++) {
				time_t t1 = time(NULL), t2;
				struct tm now = *localtime(&t1), r, n2;
				int target = (now.tm_mon + offs[k]) % 12;
				int year = now.tm_year + (target < now.tm_mon ? 1 : 0);
				const char *name = form == 0 ? gd_mon[target] : gd_mabbr[target];

				r = gd_get(name);
				t2 = time(NULL);
				n2 = *localtime(&t2);
				if (n2.tm_mon != now.tm_mon || n2.tm_year != now.tm_year)
					continue;
				gd_fields(&r, name, year, target, 1, -1, -1, -1);
				pc_check(gd_hms_now(&r, t1, t2), "%s: the time of day is not the current one", name);
				break;
			}
			if (try == 3)
				pc_unresolved("the month changed during every attempt");
		}
	}
	pc_pass();
}
