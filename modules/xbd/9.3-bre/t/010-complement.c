/*
 * A non-matching list is the exact complement of the matching list.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.010
 */
#include "rx.h"

static const char *const lists[] = {
	"abc", "a-f", "[:digit:]", "[.a.]", "[=b=]", "a-cx-z", "]a", "-a", "a-",
	"[:alpha:][:digit:]_", "a^", "\\", "[:space:]", "[:punct:]", ".*[", "]-",
	"[:upper:]a-c", "[.-.]-0",
};

static void run(int cf)
{
	size_t i;
	int c;
	char pm[64], pn[64], s[2];
	regex_t rm, rn;

	for (i = 0; i < RX_N(lists); i++) {
		snprintf(pm, sizeof pm, "[%s]", lists[i]);
		snprintf(pn, sizeof pn, "[^%s]", lists[i]);
		pc_check(regcomp(&rm, pm, cf) == 0, "regcomp(\"%s\") failed", pm);
		pc_check(regcomp(&rn, pn, cf) == 0, "regcomp(\"%s\") failed", pn);
		s[1] = '\0';
		for (c = 1; c <= 255; c++) {
			int a, b;

			s[0] = (char)c;
			a = regexec(&rm, s, 0, NULL, 0) == 0;
			b = regexec(&rn, s, 0, NULL, 0) == 0;
			pc_check(a != b, "\"%s\" and \"%s\" both %s byte 0x%02x", pm, pn,
			    a ? "match" : "reject", c);
		}
		regfree(&rm);
		regfree(&rn);
	}
}

int main(void)
{
	pc_begin("XBD.9.3-bre.010");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
