/*
 * %j reads 1 to 366 and sets tm_yday to the number minus one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.006
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.006");
	pc_test();
	sp_case("001,", "%j,", 4, SP_YDAY, (struct tm){ .tm_yday = 0 });
	sp_case("1,", "%j,", 2, SP_YDAY, (struct tm){ .tm_yday = 0 });
	sp_case("060,", "%j,", 4, SP_YDAY, (struct tm){ .tm_yday = 59 });
	sp_case("60,", "%j,", 3, SP_YDAY, (struct tm){ .tm_yday = 59 });
	sp_case("365,", "%j,", 4, SP_YDAY, (struct tm){ .tm_yday = 364 });
	sp_case("366,", "%j,", 4, SP_YDAY, (struct tm){ .tm_yday = 365 });
	sp_null("000,", "%j,");
	sp_null("367,", "%j,");
	pc_pass();
}
