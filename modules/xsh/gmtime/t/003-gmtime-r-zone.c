/*
 * gmtime_r() sets tm_zone to "UTC" (static storage) and tm_gmtoff to 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gmtime.003
 * MUTANT    gmtime.r_zone_gmt
 * MUTANT    gmtime.r_zone_buffer
 * MUTANT    gmtime.r_gmtoff
 * VARIANT   tzset.tm_members
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
	time_t t = 1234567890, u = 1600000000;
	struct tm a, b, *r;
	const char *z;

	pc_begin("XSH.gmtime.003");
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_test();
	memset(&a, 0, sizeof a);
	r = gmtime_r(&t, &a);
	pc_check(r == &a, "gmtime_r() did not return result");
	z = a.tm_zone;
	pc_check(z != NULL && strcmp(z, "UTC") == 0, "tm_zone is \"%s\", expected \"UTC\"", z ? z : "(null)");
	pc_check(a.tm_gmtoff == 0, "tm_gmtoff is %ld, expected 0", (long)a.tm_gmtoff);
	/* The string has static storage duration: it outlives further calls. */
	if (setenv("TZ", "PCE-5:30", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	(void)localtime(&u);
	pc_check(strcmp(z, "UTC") == 0, "after tzset() and localtime() the tm_zone string reads \"%s\"", z);
	r = gmtime_r(&u, &b);
	pc_check(r == &b, "the second gmtime_r() did not return result");
	pc_check(b.tm_zone != NULL && strcmp(b.tm_zone, "UTC") == 0, "the second tm_zone is \"%s\"",
	    b.tm_zone ? b.tm_zone : "(null)");
	pc_pass();
}
