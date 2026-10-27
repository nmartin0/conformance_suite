/*
 * %u reads 1 to 7 (Monday 1) and %w 0 to 6 (Sunday 0); both set tm_wday.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.013
 */
#include "sp.h"

int main(void)
{
	char buf[8];
	int d;

	pc_begin("XSH.strptime.013");
	pc_test();
	for (d = 1; d <= 7; d++) {
		(void)snprintf(buf, sizeof buf, "%d,", d);
		sp_case(buf, "%u,", 2, SP_WDAY, (struct tm){ .tm_wday = d % 7 });
	}
	for (d = 0; d <= 6; d++) {
		(void)snprintf(buf, sizeof buf, "%d,", d);
		sp_case(buf, "%w,", 2, SP_WDAY, (struct tm){ .tm_wday = d });
		sp_case(buf, "%Ow,", 2, SP_WDAY, (struct tm){ .tm_wday = d });
	}
	sp_null("0,", "%u,");
	sp_null("8,", "%u,");
	sp_null("7,", "%w,");
	pc_pass();
}
