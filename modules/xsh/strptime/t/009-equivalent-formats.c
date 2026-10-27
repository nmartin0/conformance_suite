/*
 * %D, %T, %R and %F read what their expansions read.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.009
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.009");
	pc_test();
	sp_case("02/29/24,", "%D,", 9, SP_MON | SP_MDAY | SP_YEAR, (struct tm){ .tm_mon = 1, .tm_mday = 29, .tm_year = 124 });
	sp_case("12/31/99", "%D", 8, SP_MON | SP_MDAY | SP_YEAR, (struct tm){ .tm_mon = 11, .tm_mday = 31, .tm_year = 99 });
	sp_case("13:45:59,", "%T,", 9, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 13, .tm_min = 45, .tm_sec = 59 });
	sp_case("00:00:60", "%T", 8, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 0, .tm_min = 0, .tm_sec = 60 });
	sp_case("07:08,", "%R,", 6, SP_HOUR | SP_MIN, (struct tm){ .tm_hour = 7, .tm_min = 8 });
	sp_case("2024-02-29,", "%F,", 11, SP_YEAR | SP_MON | SP_MDAY, (struct tm){ .tm_year = 124, .tm_mon = 1, .tm_mday = 29 });
	sp_case("1999-12-31", "%F", 10, SP_YEAR | SP_MON | SP_MDAY, (struct tm){ .tm_year = 99, .tm_mon = 11, .tm_mday = 31 });
	/* the year of %F has no limit on its width */
	sp_case("12345-06-07", "%F", 11, SP_YEAR | SP_MON | SP_MDAY, (struct tm){ .tm_year = 12345 - 1900, .tm_mon = 5, .tm_mday = 7 });
	sp_null("02-29-24", "%D");
	sp_null("2024/02/29", "%F");
	sp_null("13:45", "%T");
	pc_pass();
}
