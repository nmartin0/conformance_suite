/*
 * Several conversions in one format.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.012
 */
#include "fmon.h"

int main(void)
{
	char buf[128], f1[64], f2[64], dg[64];
	const char *close1, *open2;
	const char *fmt = "[%.0n][%.0i]";
	ssize_t r;
	size_t n;

	pc_begin("XSH.strfmon.012");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r = strfmon(buf, sizeof buf, fmt, 1.0, 22.0, 99.0);
	pc_check(r >= 0, "strfmon() failed, errno %s", pc_errname(errno));
	n = strlen(buf);
	pc_check((size_t)r == n, "strfmon() returned %ld but the result \"%s\" has length %zu", (long)r, buf, n);
	pc_check(n >= 6 && buf[0] == '[' && buf[n - 1] == ']', "the result \"%s\" is not two bracketed fields", buf);
	close1 = strchr(buf, ']');
	open2 = strstr(buf, "][");
	pc_check(close1 != NULL && open2 == close1, "the result \"%s\" is not two bracketed fields", buf);
	memset(f1, 0, sizeof f1);
	memset(f2, 0, sizeof f2);
	memcpy(f1, buf + 1, (size_t)(close1 - buf) - 1);
	memcpy(f2, close1 + 2, n - (size_t)(close1 - buf) - 3);
	fm_digits(f1, dg, sizeof dg);
	pc_check(strcmp(dg, "1") == 0, "the first field \"%s\" has digits \"%s\", expected 1", f1, dg);
	fm_digits(f2, dg, sizeof dg);
	pc_check(strcmp(dg, "22") == 0, "the second field \"%s\" has digits \"%s\", expected 22", f2, dg);
	pc_pass();
}
