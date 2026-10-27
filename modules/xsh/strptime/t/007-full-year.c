/*
 * %Y sets tm_year to the year minus 1900 and reads at most four characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.007
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.007");
	pc_test();
	sp_case("1999,", "%Y,", 5, SP_YEAR, (struct tm){ .tm_year = 99 });
	sp_case("2024,", "%Y,", 5, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("1900,", "%Y,", 5, SP_YEAR, (struct tm){ .tm_year = 0 });
	sp_case("0070,", "%Y,", 5, SP_YEAR, (struct tm){ .tm_year = 70 - 1900 });
	sp_case("2024", "%Y", 4, SP_YEAR, (struct tm){ .tm_year = 124 });
	/* only four characters are scanned: the rest stays unread */
	sp_case("123456", "%Y", 4, SP_YEAR, (struct tm){ .tm_year = 1234 - 1900 });
	pc_pass();
}
