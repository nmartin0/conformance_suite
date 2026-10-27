/*
 * In the POSIX locale %c, %x, %X and %r read the formats of that locale's date and time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.010
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.010");
	pc_test();
	sp_case("Thu Feb 29 13:45:59 2024", "%c", 24, SP_WDAY | SP_MON | SP_MDAY | SP_HOUR | SP_MIN | SP_SEC | SP_YEAR,
	    (struct tm){ .tm_wday = 4, .tm_mon = 1, .tm_mday = 29, .tm_hour = 13, .tm_min = 45, .tm_sec = 59, .tm_year = 124 });
	sp_case("Mon Mar  4 01:02:03 2024", "%c", 24, SP_WDAY | SP_MON | SP_MDAY | SP_HOUR | SP_MIN | SP_SEC | SP_YEAR,
	    (struct tm){ .tm_wday = 1, .tm_mon = 2, .tm_mday = 4, .tm_hour = 1, .tm_min = 2, .tm_sec = 3, .tm_year = 124 });
	sp_case("Fri Dec 31 23:59:60 1999,", "%c,", 25, SP_WDAY | SP_MON | SP_MDAY | SP_HOUR | SP_MIN | SP_SEC | SP_YEAR,
	    (struct tm){ .tm_wday = 5, .tm_mon = 11, .tm_mday = 31, .tm_hour = 23, .tm_min = 59, .tm_sec = 60, .tm_year = 99 });
	sp_case("02/29/24", "%x", 8, SP_MON | SP_MDAY | SP_YEAR, (struct tm){ .tm_mon = 1, .tm_mday = 29, .tm_year = 124 });
	sp_case("07/04/76,", "%x,", 9, SP_MON | SP_MDAY | SP_YEAR, (struct tm){ .tm_mon = 6, .tm_mday = 4, .tm_year = 76 });
	sp_case("13:45:59", "%X", 8, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 13, .tm_min = 45, .tm_sec = 59 });
	sp_case("01:02:03 PM", "%r", 11, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 13, .tm_min = 2, .tm_sec = 3 });
	sp_case("12:00:00 AM", "%r", 11, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 0, .tm_min = 0, .tm_sec = 0 });
	sp_case("12:30:45 pm,", "%r,", 12, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 12, .tm_min = 30, .tm_sec = 45 });
	sp_null("Thu Feb 29 13:45:59", "%c");
	sp_null("13:45:59", "%r");
	pc_pass();
}
