/*
 * dbm_delete() removes a record and its key; the others stay.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.006
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	char k[32], v[64];
	int seen[50] = { 0 }, i;
	long n, dups, others;

	pc_begin("XSH.dbm_open.006");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	for (i = 0; i < 50; i++) {
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		pc_check(dbm_store(db, dbx_str(k), dbx_str(v), DBM_INSERT) == 0, "store %d failed", i);
	}
	pc_test();
	for (i = 0; i < 50; i += 2) {
		dbx_key(k, sizeof k, i);
		pc_check(dbm_delete(db, dbx_str(k)) == 0, "dbm_delete of key %d did not return 0", i);
	}
	for (i = 0; i < 50; i++) {
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		if (i % 2 == 0)
			pc_check(dbm_fetch(db, dbx_str(k)).dptr == NULL, "deleted key %d can still be fetched", i);
		else
			pc_check(dbx_has(db, dbx_str(k), v, strlen(v)), "kept key %d lost its content", i);
	}
	n = dbx_walk(db, seen, 50, &dups, &others);
	pc_check(n == 25 && dups == 0 && others == 0, "the walk visited %ld keys, not the 25 kept", n);
	for (i = 0; i < 50; i++)
		pc_check(seen[i] == (i % 2), "key %d was visited %d times", i, seen[i]);
	dbm_close(db);
	pc_pass();
}
