/*
 * White space in the format, %n and %t skip any run of white space in the input, including none.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.011
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.011");
	pc_test();
	sp_pos(" \t\n x", "%n", 4);
	sp_pos(" \t\n x", "%t", 4);
	sp_pos(" \t\n x", " ", 4);
	sp_pos(" \t\n x", "%n%t ", 4);
	sp_pos("x", "%n", 0);
	sp_pos("", "%t", 0);
	sp_pos("a \t b", "a%tb", 5);
	sp_pos("ab", "a%tb", 2);
	sp_pos("a   b", "a b", 5);
	sp_pos("a\nb", "a%nb", 3);
	sp_case("3  \t 4", "%d %d", 6, SP_MDAY, (struct tm){ .tm_mday = 4 });
	sp_case("   7", " %d", 4, SP_MDAY, (struct tm){ .tm_mday = 7 });
	sp_case("7   x", "%d ", 4, SP_MDAY, (struct tm){ .tm_mday = 7 });
	sp_case(" \t 5,", "%n%t%d,", 5, SP_MDAY, (struct tm){ .tm_mday = 5 });
	pc_pass();
}
