/*
 * Input that does not match the format gives a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.019
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.019");
	pc_test();
	/* control: a valid input is converted, so a null result for a bad one is a judgement */
	{
		struct tm tm = sp_blank();
		static const char ok[] = "12:30";
		char *e = strptime(ok, "%H:%M", &tm);

		pc_check(e == ok + 5 && tm.tm_hour == 12 && tm.tm_min == 30, "strptime(\"12:30\", \"%%H:%%M\") did not convert");
	}
	sp_null("Foo,", "%a,");
	sp_null("Sundae,", "%A,");
	sp_null("Janu,", "%b,");
	sp_null("Smarch,", "%B,");
	sp_null("", "%a");
	sp_null("", "%b");
	sp_null("", "%d");
	sp_null("x", "%d");
	sp_null("12", "%d:%m");
	sp_null("12/", "%d-");
	sp_null("12:xx", "%H:%M");
	sp_null("abcd", "%Y");
	sp_null("xx", "%y");
	sp_null("AX", "%p");
	sp_null("", "%H");
	sp_null("12 30", "%H:%M");
	pc_pass();
}
