/*
 * With a left precision the positive and negative formats are equally long.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.011
 */
#include "fmon.h"

int main(void)
{
	char p[128], n[128];

	pc_begin("XSH.strfmon.011");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	fm_ok(p, sizeof p, "%#5.0n", 123.0);
	fm_ok(n, sizeof n, "%#5.0n", -123.0);
	pc_check(strlen(p) == strlen(n), "\"%%#5.0n\" gave \"%s\" for 123.0 and \"%s\" for -123.0, of different lengths", p, n);
	fm_ok(p, sizeof p, "%=*#6.0i", 7.0);
	fm_ok(n, sizeof n, "%=*#6.0i", -7.0);
	pc_check(strlen(p) == strlen(n), "\"%%=*#6.0i\" gave \"%s\" for 7.0 and \"%s\" for -7.0, of different lengths", p, n);
	pc_pass();
}
