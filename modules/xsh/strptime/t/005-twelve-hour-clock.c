/*
 * %I together with %p, in either order, sets tm_hour on the 24-hour clock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.005
 */
#include "sp.h"

int main(void)
{
	static const struct { const char *ip; const char *pi; int hour; } t[] = {
		{ "12 AM", "AM 12", 0 }, { "01 AM", "AM 01", 1 }, { "11 AM", "AM 11", 11 },
		{ "12 PM", "PM 12", 12 }, { "01 PM", "PM 01", 13 }, { "11 PM", "PM 11", 23 },
		{ "6 pm", "pm 6", 18 }, { "9 Am", "aM 9", 9 },
	};
	size_t i;

	pc_begin("XSH.strptime.005");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		sp_case(t[i].ip, "%I %p", (long)strlen(t[i].ip), SP_HOUR, (struct tm){ .tm_hour = t[i].hour });
		sp_case(t[i].pi, "%p %I", (long)strlen(t[i].pi), SP_HOUR, (struct tm){ .tm_hour = t[i].hour });
		sp_case(t[i].ip, "%OI %p", (long)strlen(t[i].ip), SP_HOUR, (struct tm){ .tm_hour = t[i].hour });
	}
	sp_null("13 AM", "%I %p");
	sp_null("0 AM", "%I %p");
	pc_pass();
}
