/*
 * DBM_INSERT and DBM_REPLACE are defined with different values that fit
 * the int store_mode argument of dbm_store().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ndbm.h.004
 */
#include "pc.h"
#include <ndbm.h>

#if defined(DBM_INSERT) && defined(DBM_REPLACE)
/* integer constant expressions, and values an int can hold */
enum { pc_ins = (DBM_INSERT), pc_rep = (DBM_REPLACE) };
#endif

int main(void)
{
	pc_begin("XBD.ndbm.h.004");
#if !defined(DBM_INSERT) || !defined(DBM_REPLACE)
	pc_unresolved("a constant is missing (the header check reports it): DBM_INSERT DBM_REPLACE");
#else
	pc_test();
	pc_check(pc_ins != pc_rep, "DBM_INSERT and DBM_REPLACE are both %d", pc_ins);
	pc_pass();
#endif
}
