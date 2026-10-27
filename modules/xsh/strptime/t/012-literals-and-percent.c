/*
 * Ordinary characters and %% must match exactly; a mismatch fails.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.012
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.012");
	pc_test();
	sp_case("7-8-2024", "%d-%m-%Y", 8, SP_MDAY | SP_MON | SP_YEAR, (struct tm){ .tm_mday = 7, .tm_mon = 7, .tm_year = 124 });
	sp_case("[12:30]", "[%H:%M]", 7, SP_HOUR | SP_MIN, (struct tm){ .tm_hour = 12, .tm_min = 30 });
	sp_case("30%", "%d%%", 3, SP_MDAY, (struct tm){ .tm_mday = 30 });
	sp_pos("x%y", "x%%y", 3);
	sp_pos("abc", "ab", 2);
	sp_null("7/8", "%d-%m");
	sp_null("30x", "%d%%");
	sp_null("a", "%%");
	sp_null("A", "a");
	sp_null("ab", "ac");
	sp_null("", "a");
	pc_pass();
}
