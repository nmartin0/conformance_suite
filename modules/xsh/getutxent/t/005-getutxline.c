/*
 * getutxline() finds a login or user process entry with the requested line.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.005
 */
#include "utx.h"

int main(void)
{
	struct utmpx e[UTX_MAX], q, *r;
	long n, kept, i, tested = 0;

	pc_begin("XSH.getutxent.005");
	pc_test();
	n = utx_read_all(e, &kept);
	if (n == 0)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	for (i = 0; i < kept && tested < 20; i++) {
		if ((e[i].ut_type != USER_PROCESS && e[i].ut_type != LOGIN_PROCESS) || e[i].ut_line[0] == '\0')
			continue;
		memset(&q, 0, sizeof q);
		q.ut_type = e[i].ut_type;
		memcpy(q.ut_line, e[i].ut_line, sizeof q.ut_line);
		setutxent();
		r = getutxline(&q);
		pc_check(r != NULL, "no entry found for a line that the database holds");
		pc_check(r->ut_type == USER_PROCESS || r->ut_type == LOGIN_PROCESS,
		    "the entry found has type %d", (int)r->ut_type);
		pc_check(strncmp(r->ut_line, q.ut_line, sizeof q.ut_line) == 0, "the entry found has another line");
		tested++;
	}
	endutxent();
	if (tested == 0)
		pc_notinuse("the database holds no login or user process entry with a line");
	pc_pass();
}
