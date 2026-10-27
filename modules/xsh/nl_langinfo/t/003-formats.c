/*
 * AM/PM strings and format strings in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.003
 */
#include "lc.h"

int main(void)
{
	pc_begin("XSH.nl_langinfo.003");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	pc_check(strcmp(nl_langinfo(AM_STR), "AM") == 0, "AM_STR is \"%s\"", nl_langinfo(AM_STR));
	pc_check(strcmp(nl_langinfo(PM_STR), "PM") == 0, "PM_STR is \"%s\"", nl_langinfo(PM_STR));
	pc_check(strcmp(nl_langinfo(D_T_FMT), "%a %b %e %H:%M:%S %Y") == 0, "D_T_FMT is \"%s\"", nl_langinfo(D_T_FMT));
	pc_check(strcmp(nl_langinfo(D_FMT), "%m/%d/%y") == 0, "D_FMT is \"%s\"", nl_langinfo(D_FMT));
	pc_check(strcmp(nl_langinfo(T_FMT), "%H:%M:%S") == 0, "T_FMT is \"%s\"", nl_langinfo(T_FMT));
	pc_check(strcmp(nl_langinfo(T_FMT_AMPM), "%I:%M:%S %p") == 0, "T_FMT_AMPM is \"%s\"", nl_langinfo(T_FMT_AMPM));
	pc_pass();
}
