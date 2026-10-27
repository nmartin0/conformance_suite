/*
 * %m, %H, %M and %S read their fields within the stated ranges; values outside fail.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.004
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.004");
	pc_test();
	sp_case("1,", "%m,", 2, SP_MON, (struct tm){ .tm_mon = 0 });
	sp_case("09,", "%m,", 3, SP_MON, (struct tm){ .tm_mon = 8 });
	sp_case("12,", "%m,", 3, SP_MON, (struct tm){ .tm_mon = 11 });
	sp_case("6,", "%Om,", 2, SP_MON, (struct tm){ .tm_mon = 5 });
	sp_null("13,", "%m,");
	sp_null("0,", "%m,");
	sp_null("00,", "%m,");
	sp_case("0,", "%H,", 2, SP_HOUR, (struct tm){ .tm_hour = 0 });
	sp_case("00,", "%H,", 3, SP_HOUR, (struct tm){ .tm_hour = 0 });
	sp_case("7,", "%H,", 2, SP_HOUR, (struct tm){ .tm_hour = 7 });
	sp_case("23,", "%H,", 3, SP_HOUR, (struct tm){ .tm_hour = 23 });
	sp_case("21,", "%OH,", 3, SP_HOUR, (struct tm){ .tm_hour = 21 });
	sp_null("24,", "%H,");
	sp_case("0,", "%M,", 2, SP_MIN, (struct tm){ .tm_min = 0 });
	sp_case("05,", "%M,", 3, SP_MIN, (struct tm){ .tm_min = 5 });
	sp_case("59,", "%M,", 3, SP_MIN, (struct tm){ .tm_min = 59 });
	sp_case("45,", "%OM,", 3, SP_MIN, (struct tm){ .tm_min = 45 });
	sp_null("60,", "%M,");
	sp_case("0,", "%S,", 2, SP_SEC, (struct tm){ .tm_sec = 0 });
	sp_case("07,", "%S,", 3, SP_SEC, (struct tm){ .tm_sec = 7 });
	sp_case("59,", "%S,", 3, SP_SEC, (struct tm){ .tm_sec = 59 });
	sp_case("60,", "%S,", 3, SP_SEC, (struct tm){ .tm_sec = 60 });
	sp_case("33,", "%OS,", 3, SP_SEC, (struct tm){ .tm_sec = 33 });
	sp_null("61,", "%S,");
	pc_pass();
}
