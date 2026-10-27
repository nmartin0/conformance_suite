/*
 * O_TRUNC empties an existing database.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.011
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	char k[32], v[64];
	int i;

	pc_begin("XSH.dbm_open.011");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	for (i = 0; i < 5; i++) {
		dbx_key(k, sizeof k, i);
		dbx_val(v, sizeof v, i);
		pc_check(dbm_store(db, dbx_str(k), dbx_str(v), DBM_INSERT) == 0, "store %d failed", i);
	}
	dbm_close(db);
	pc_test();
	db = dbm_open(DBN, O_RDWR | O_TRUNC, 0600);
	pc_check(db != NULL, "dbm_open(O_TRUNC) of an existing database failed");
	pc_check(dbm_firstkey(db).dptr == NULL, "the database still has a key after O_TRUNC");
	dbx_key(k, sizeof k, 2);
	pc_check(dbm_fetch(db, dbx_str(k)).dptr == NULL, "a record survived O_TRUNC");
	dbm_close(db);
	pc_pass();
}
