/*
 * A field width limits the bytes converted by C, F, G and Y; the flags 0 and + are ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.017
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.017");
	pc_test();
	sp_case("2024", "%2Y", 2, SP_YEAR, (struct tm){ .tm_year = 20 - 1900 });
	sp_case("202401", "%6Y", 6, SP_YEAR, (struct tm){ .tm_year = 202401 - 1900 });
	sp_case("202401", "%+6Y", 6, SP_YEAR, (struct tm){ .tm_year = 202401 - 1900 });
	sp_case("202401", "%06Y", 6, SP_YEAR, (struct tm){ .tm_year = 202401 - 1900 });
	sp_case("2024", "%+Y", 4, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("2024", "%0Y", 4, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("2024", "%4Y", 4, SP_YEAR, (struct tm){ .tm_year = 124 });
	sp_case("19", "%2C", 2, SP_YEAR, (struct tm){ .tm_year = 0 });
	sp_case("1998", "%4G", 4, 0, (struct tm){ 0 });
	sp_case("2024-02-29", "%10F", 10, SP_YEAR | SP_MON | SP_MDAY, (struct tm){ .tm_year = 124, .tm_mon = 1, .tm_mday = 29 });
	pc_pass();
}
