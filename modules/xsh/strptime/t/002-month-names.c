/*
 * %b, %B and %h (also with the O modifier) read month names, in any case, and set tm_mon.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.002
 */
#include "sp.h"

int main(void)
{
	static const char *const ab[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	static const char *const fu[12] = { "January", "February", "March", "April", "May", "June", "July", "August",
	    "September", "October", "November", "December" };
	static const char *const fmts[] = { "%b,", "%B,", "%h,", "%Ob,", "%OB,", "%Oh," };
	char buf[64], up[64];
	int m;
	size_t f, i;

	pc_begin("XSH.strptime.002");
	pc_test();
	for (m = 0; m < 12; m++) {
		for (f = 0; f < sizeof fmts / sizeof fmts[0]; f++) {
			(void)snprintf(buf, sizeof buf, "%s,x", ab[m]);
			sp_case(buf, fmts[f], (long)strlen(ab[m]) + 1, SP_MON, (struct tm){ .tm_mon = m });
			(void)snprintf(buf, sizeof buf, "%s,x", fu[m]);
			sp_case(buf, fmts[f], (long)strlen(fu[m]) + 1, SP_MON, (struct tm){ .tm_mon = m });
			for (i = 0; fu[m][i] != '\0'; i++)
				up[i] = (char)((fu[m][i] >= 'a' && fu[m][i] <= 'z') ? fu[m][i] - 'a' + 'A' : fu[m][i]);
			up[i] = '\0';
			(void)snprintf(buf, sizeof buf, "%s,x", up);
			sp_case(buf, fmts[f], (long)strlen(up) + 1, SP_MON, (struct tm){ .tm_mon = m });
		}
		sp_case(fu[m], "%B", (long)strlen(fu[m]), SP_MON, (struct tm){ .tm_mon = m });
	}
	pc_pass();
}
