/*
 * %a and %A read abbreviated or full weekday names, in any case, and set tm_wday.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.001
 */
#include "sp.h"

int main(void)
{
	static const char *const ab[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char *const fu[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	static const char *const odd[7] = { "sUN", "mON", "TUE", "wed", "tHu", "FRI", "sAt" };
	char buf[64];
	int d;

	pc_begin("XSH.strptime.001");
	pc_test();
	for (d = 0; d < 7; d++) {
		(void)snprintf(buf, sizeof buf, "%s,x", ab[d]);
		sp_case(buf, "%a,", (long)strlen(ab[d]) + 1, SP_WDAY, (struct tm){ .tm_wday = d });
		sp_case(buf, "%A,", (long)strlen(ab[d]) + 1, SP_WDAY, (struct tm){ .tm_wday = d });
		(void)snprintf(buf, sizeof buf, "%s,x", fu[d]);
		sp_case(buf, "%a,", (long)strlen(fu[d]) + 1, SP_WDAY, (struct tm){ .tm_wday = d });
		sp_case(buf, "%A,", (long)strlen(fu[d]) + 1, SP_WDAY, (struct tm){ .tm_wday = d });
		(void)snprintf(buf, sizeof buf, "%s,x", odd[d]);
		sp_case(buf, "%a,", (long)strlen(odd[d]) + 1, SP_WDAY, (struct tm){ .tm_wday = d });
		(void)snprintf(buf, sizeof buf, "%s", fu[d]);
		sp_case(buf, "%A", (long)strlen(fu[d]), SP_WDAY, (struct tm){ .tm_wday = d });
		(void)snprintf(buf, sizeof buf, "%s", ab[d]);
		sp_case(buf, "%a", (long)strlen(ab[d]), SP_WDAY, (struct tm){ .tm_wday = d });
	}
	pc_pass();
}
