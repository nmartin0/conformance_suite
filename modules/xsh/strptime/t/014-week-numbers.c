/*
 * %U, %W, %V, %G and %g are read within their ranges; the ISO week examples set tm_wday through %u.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.014
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.014");
	pc_test();
	sp_pos("00,", "%U,", 3);
	sp_pos("53,", "%U,", 3);
	sp_pos("7,", "%W,", 2);
	sp_pos("00,", "%W,", 3);
	sp_pos("53,", "%W,", 3);
	sp_pos("01,", "%V,", 3);
	sp_pos("53,", "%V,", 3);
	sp_pos("12,", "%OU,", 3);
	sp_pos("12,", "%OW,", 3);
	sp_pos("12,", "%OV,", 3);
	sp_null("54,", "%U,");
	sp_null("54,", "%W,");
	sp_null("00,", "%V,");
	sp_null("54,", "%V,");
	sp_pos("1998,", "%G,", 5);
	sp_pos("77,", "%g,", 3);
	sp_case("1998 53 6", "%G %V %u", 9, SP_WDAY, (struct tm){ .tm_wday = 6 });
	sp_case("1998 01 2", "%G %V %u", 9, SP_WDAY, (struct tm){ .tm_wday = 2 });
	pc_pass();
}
