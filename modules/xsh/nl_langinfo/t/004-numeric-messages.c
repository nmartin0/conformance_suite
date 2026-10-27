/*
 * RADIXCHAR, YESEXPR and NOEXPR in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.004
 */
#include "lc.h"

int main(void)
{
	pc_begin("XSH.nl_langinfo.004");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	pc_check(strcmp(nl_langinfo(RADIXCHAR), ".") == 0, "RADIXCHAR is \"%s\"", nl_langinfo(RADIXCHAR));
	pc_check(strcmp(nl_langinfo(YESEXPR), "^[yY]") == 0, "YESEXPR is \"%s\"", nl_langinfo(YESEXPR));
	pc_check(strcmp(nl_langinfo(NOEXPR), "^[nN]") == 0, "NOEXPR is \"%s\"", nl_langinfo(NOEXPR));
	pc_pass();
}
