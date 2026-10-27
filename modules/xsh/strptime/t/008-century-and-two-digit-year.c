/*
 * %y chooses the century by the 69 to 99 / 00 to 68 rule; %C changes it, in either order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.008
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.008");
	pc_test();
	sp_case("69,", "%y,", 3, SP_YEAR, (struct tm){ .tm_year = 69 });
	sp_case("99,", "%y,", 3, SP_YEAR, (struct tm){ .tm_year = 99 });
	sp_case("00,", "%y,", 3, SP_YEAR, (struct tm){ .tm_year = 100 });
	sp_case("05,", "%y,", 3, SP_YEAR, (struct tm){ .tm_year = 105 });
	sp_case("1,", "%y,", 2, SP_YEAR, (struct tm){ .tm_year = 101 });
	sp_case("68,", "%y,", 3, SP_YEAR, (struct tm){ .tm_year = 168 });
	sp_case("24,", "%Oy,", 3, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("19,", "%C,", 3, SP_YEAR, (struct tm){ .tm_year = 0 });
	sp_case("20,", "%C,", 3, SP_YEAR, (struct tm){ .tm_year = 100 });
	sp_case("21,", "%C,", 3, SP_YEAR, (struct tm){ .tm_year = 200 });
	sp_case("20 24", "%C %y", 5, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("24 20", "%y %C", 5, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("20 69", "%C %y", 5, SP_YEAR, (struct tm){ .tm_year = 169 });
	sp_case("69 20", "%y %C", 5, SP_YEAR, (struct tm){ .tm_year = 169 });
	sp_case("19 05", "%C %y", 5, SP_YEAR, (struct tm){ .tm_year = 5 });
	pc_pass();
}
