/*
 * ctime() of the Epoch in a zone with no offset is the asctime() string for 1 January 1970.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctime.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
	time_t t = 0;
	char *p;

	pc_begin("XSH.ctime.001");
	pc_check(setenv("TZ", "UTC0", 1) == 0, "setenv failed");
	tzset();
	pc_test();
	p = ctime(&t);
	pc_check(p != NULL, "ctime returned a null pointer");
	pc_check(strcmp(p, "Thu Jan  1 00:00:00 1970\n") == 0, "ctime(0) produced \"%s\"", p);
	pc_pass();
}
