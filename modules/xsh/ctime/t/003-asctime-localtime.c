/*
 * ctime(t) is the same string as asctime(localtime(t)).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctime.003
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
	static const long tv[] = { 0L, 1L, 86400L, 951782400L, 1000000000L, 2000000000L };
	static const char *const zones[] = { "UTC0", "EST5", "ABC-3" };
	char a[64];
	size_t i, z;

	pc_begin("XSH.ctime.003");
	pc_test();
	for (z = 0; z < 3; z++) {
		pc_check(setenv("TZ", zones[z], 1) == 0, "setenv failed");
		tzset();
		for (i = 0; i < sizeof tv / sizeof tv[0]; i++) {
			time_t t = (time_t)tv[i];
			char *p = ctime(&t);
			struct tm *tm;
			char *q;

			pc_check(p != NULL, "ctime returned a null pointer");
			pc_check(strlen(p) < sizeof a, "ctime string too long");
			strcpy(a, p);
			tm = localtime(&t);
			pc_check(tm != NULL, "localtime returned a null pointer");
			q = asctime(tm);
			pc_check(q != NULL, "asctime returned a null pointer");
			pc_check(strcmp(a, q) == 0, "TZ=%s t=%ld: ctime \"%s\" but asctime(localtime) \"%s\"",
			    zones[z], tv[i], a, q);
		}
	}
	pc_pass();
}
