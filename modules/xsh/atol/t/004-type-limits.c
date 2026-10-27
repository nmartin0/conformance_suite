/*
 * atol: type limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atol.004
 */
#include "pc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
static long (*volatile f_atol)(const char *) = atol;
static long long (*volatile f_atoll)(const char *) = atoll;

int main(void)
{
	char b[48];
	pc_begin("XSH.atol.004");
	pc_test();
	pc_check(snprintf(b, sizeof b, "%ld", LONG_MAX) > 0, "snprintf failed");
	pc_check(f_atol(b) == LONG_MAX, "atol(\"%s\") did not give LONG_MAX", b);
	pc_check(snprintf(b, sizeof b, "%ld", LONG_MIN) > 0, "snprintf failed");
	pc_check(f_atol(b) == LONG_MIN, "atol(\"%s\") did not give LONG_MIN", b);
	pc_check(snprintf(b, sizeof b, "%lld", LLONG_MAX) > 0, "snprintf failed");
	pc_check(f_atoll(b) == LLONG_MAX, "atoll(\"%s\") did not give LLONG_MAX", b);
	pc_check(snprintf(b, sizeof b, "%lld", LLONG_MIN) > 0, "snprintf failed");
	pc_check(f_atoll(b) == LLONG_MIN, "atoll(\"%s\") did not give LLONG_MIN", b);
	pc_pass();
}
