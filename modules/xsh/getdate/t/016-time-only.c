/*
 * With only a time of day the date is the first moment from the current hour on at which that hour and minute occur.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.016
 */
#include "gd.h"

int main(void)
{
	static const int ahead[] = { 3, 23 };
	size_t k;
	int try;

	pc_begin("XSH.getdate.016");
	gd_setup("%H:%M\n");
	pc_test();
	for (k = 0; k < GD_N(ahead); k++) {
		for (try = 0; try < 3; try++) {
			time_t t1 = time(NULL), t2, tr;
			struct tm now = *localtime(&t1), r, n2;
			int hour = (now.tm_hour + ahead[k]) % 24;
			char in[16];

			snprintf(in, sizeof in, "%02d:30", hour);
			r = gd_get(in);
			t2 = time(NULL);
			n2 = *localtime(&t2);
			if (n2.tm_hour != now.tm_hour || !gd_same_day(t1, t2))
				continue;
			gd_fields(&r, in, -1, -1, -1, hour, 30, 0);
			r.tm_isdst = 0;
			tr = mktime(&r);
			pc_check(tr != (time_t)-1, "%s: the result cannot be converted", in);
			pc_check(tr > t1 && tr <= t2 + (time_t)ahead[k] * 3600 + 3600,
			    "%s: the result is %ld s from now, hour %d ahead", in, (long)(tr - t1), ahead[k]);
			if (ahead[k] == 23)
				pc_check(tr > t1 + 20 * 3600, "%s: the result is only %ld s ahead", in,
				    (long)(tr - t1));
			break;
		}
		if (try == 3)
			pc_unresolved("the hour changed during every attempt");
	}
	pc_pass();
}
