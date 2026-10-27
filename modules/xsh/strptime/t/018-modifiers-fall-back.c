/*
 * With no alternative formats or digits in the POSIX locale, the E and O modifiers act as the plain conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.018
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.018");
	pc_test();
	sp_case("Thu Feb 29 13:45:59 2024", "%Ec", 24, SP_WDAY | SP_MON | SP_MDAY | SP_HOUR | SP_MIN | SP_SEC | SP_YEAR,
	    (struct tm){ .tm_wday = 4, .tm_mon = 1, .tm_mday = 29, .tm_hour = 13, .tm_min = 45, .tm_sec = 59, .tm_year = 124 });
	sp_case("02/29/24", "%Ex", 8, SP_MON | SP_MDAY | SP_YEAR, (struct tm){ .tm_mon = 1, .tm_mday = 29, .tm_year = 124 });
	sp_case("13:45:59", "%EX", 8, SP_HOUR | SP_MIN | SP_SEC, (struct tm){ .tm_hour = 13, .tm_min = 45, .tm_sec = 59 });
	sp_case("20,", "%EC,", 3, SP_YEAR, (struct tm){ .tm_year = 100 });
	sp_case("99,", "%Ey,", 3, SP_YEAR, (struct tm){ .tm_year = 99 });
	sp_case("2024,", "%EY,", 5, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("5,", "%Od,", 2, SP_MDAY, (struct tm){ .tm_mday = 5 });
	sp_case("6,", "%Oe,", 2, SP_MDAY, (struct tm){ .tm_mday = 6 });
	sp_case("13,", "%OH,", 3, SP_HOUR, (struct tm){ .tm_hour = 13 });
	sp_case("11 PM", "%OI %p", 5, SP_HOUR, (struct tm){ .tm_hour = 23 });
	sp_case("12,", "%Om,", 3, SP_MON, (struct tm){ .tm_mon = 11 });
	sp_case("45,", "%OM,", 3, SP_MIN, (struct tm){ .tm_min = 45 });
	sp_case("59,", "%OS,", 3, SP_SEC, (struct tm){ .tm_sec = 59 });
	sp_case("3,", "%Ow,", 2, SP_WDAY, (struct tm){ .tm_wday = 3 });
	sp_case("05,", "%Oy,", 3, SP_YEAR, (struct tm){ .tm_year = 105 });
	pc_pass();
}
