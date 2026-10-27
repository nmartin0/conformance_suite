/*
 * Records survive dbm_close() and can be read through a read-only open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.009
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	char k[32], v[64];
	int seen[30] = { 0 }, i;
	long n, dups, others;

	pc_begin("XSH.dbm_open.009");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	for (i = 0; i < 30; i++) {
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		pc_check(dbm_store(db, dbx_str(k), dbx_str(v), DBM_INSERT) == 0, "store %d failed", i);
	}
	dbm_close(db);
	pc_test();
	db = dbm_open(DBN, O_RDONLY, 0);
	pc_check(db != NULL, "dbm_open(O_RDONLY) of an existing database failed");
	for (i = 0; i < 30; i++) {
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		pc_check(dbx_has(db, dbx_str(k), v, strlen(v)), "key %d lost its content", i);
	}
	n = dbx_walk(db, seen, 30, &dups, &others);
	pc_check(n == 30 && dups == 0 && others == 0, "the walk visited %ld keys, not 30", n);
	dbm_close(db);
	db = dbm_open(DBN, O_RDWR, 0);
	pc_check(db != NULL, "dbm_open(O_RDWR) of an existing database failed");
	dbx_key(k, sizeof k, 7);
	dbx_val(v, sizeof v, 7);
	pc_check(dbx_has(db, dbx_str(k), v, strlen(v)), "key 7 lost its content after reopening");
	dbm_close(db);
	pc_pass();
}
