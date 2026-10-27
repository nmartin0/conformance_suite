/*
 * The key iteration visits every key of 200 records exactly once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.007
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	char k[32], v[64];
	int seen[200] = { 0 }, i;
	long n, dups, others;

	pc_begin("XSH.dbm_open.007");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_test();
	for (i = 0; i < 200; i++) {
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		if (dbm_store(db, dbx_str(k), dbx_str(v), DBM_INSERT) != 0) {
			if ((long)(i + 1) * (long)(strlen(k) + strlen(v)) <= 1023)
				pc_fail("store %d failed although all records so far fit in one block", i);
			pc_unresolved("store %d refused: only one block of data is guaranteed (colliding keys may fill a block)", i);
		}
	}
	n = dbx_walk(db, seen, 200, &dups, &others);
	pc_check(n == 200, "the walk visited %ld keys, not 200", n);
	pc_check(dups == 0 && others == 0, "the walk repeated %ld keys and returned %ld unknown keys", dups, others);
	for (i = 0; i < 200; i++) {
		pc_check(seen[i] == 1, "key %d was visited %d times", i, seen[i]);
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		pc_check(dbx_has(db, dbx_str(k), v, strlen(v)), "key %d has other content", i);
	}
	pc_check(dbm_error(db) == 0, "the error condition is set");
	dbm_close(db);
	pc_pass();
}
