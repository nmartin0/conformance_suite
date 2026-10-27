/*
 * After the end of the database has been reached, getutxid() and getutxline() find nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.004
 */
#include "utx.h"

int main(void)
{
	struct utmpx e[UTX_MAX], q;
	long n, kept;

	pc_begin("XSH.getutxent.004");
	pc_test();
	n = utx_read_all(e, &kept);
	if (n == 0)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	memset(&q, 0, sizeof q);
	q.ut_type = USER_PROCESS;
	memset(q.ut_id, 'Z', sizeof q.ut_id);	/* an id no entry read carries, so a cached entry cannot match */
	pc_check(getutxid(&q) == NULL, "getutxid() found an entry after the end was reached");
	memset(&q, 0, sizeof q);
	q.ut_type = USER_PROCESS;
	memcpy(q.ut_line, "pcnoline", sizeof q.ut_line < 8 ? sizeof q.ut_line : 8);
	pc_check(getutxline(&q) == NULL, "getutxline() found an entry after the end was reached");
	endutxent();
	pc_pass();
}
