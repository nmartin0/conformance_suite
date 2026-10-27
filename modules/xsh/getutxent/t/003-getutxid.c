/*
 * getutxid() finds an entry of the requested type, or of the four process types with the requested id.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.003
 */
#include "utx.h"

int main(void)
{
	struct utmpx e[UTX_MAX], q, *r;
	long n, kept, i, tested = 0;

	pc_begin("XSH.getutxent.003");
	pc_test();
	n = utx_read_all(e, &kept);
	if (n == 0)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	for (i = 0; i < kept && tested < 40; i++) {
		int t = e[i].ut_type;

		if (t == BOOT_TIME || t == OLD_TIME || t == NEW_TIME) {
			memset(&q, 0, sizeof q);
			q.ut_type = (short)t;
			setutxent();
			r = getutxid(&q);
			pc_check(r != NULL, "no entry found for type %d that the database holds", t);
			pc_check(r->ut_type == t, "the entry found has type %d, not %d", (int)r->ut_type, t);
			tested++;
		} else if (utx_proc_type(t)) {
			memset(&q, 0, sizeof q);
			q.ut_type = (short)t;
			memcpy(q.ut_id, e[i].ut_id, sizeof q.ut_id);
			setutxent();
			r = getutxid(&q);
			pc_check(r != NULL, "no entry found for an id that the database holds");
			pc_check(utx_proc_type(r->ut_type), "the entry found has type %d, not a process type", (int)r->ut_type);
			pc_check(strncmp(r->ut_id, q.ut_id, sizeof q.ut_id) == 0, "the entry found has another id");
			tested++;
		}
	}
	endutxent();
	if (tested == 0)
		pc_notinuse("the database holds no entry of a searchable type");
	pc_pass();
}
