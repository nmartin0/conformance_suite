/*
 * A database opened with O_WRONLY accepts stores.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.015
 */
#include "dbx.h"

int main(void)
{
	DBM *db;

	pc_begin("XSH.dbm_open.015");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_check(dbm_store(db, dbx_str("one"), dbx_str("1"), DBM_INSERT) == 0, "store failed");
	dbm_close(db);
	pc_test();
	db = dbm_open(DBN, O_WRONLY, 0);
	pc_check(db != NULL, "dbm_open(O_WRONLY) of an existing database failed");
	pc_check(dbm_store(db, dbx_str("two"), dbx_str("22"), DBM_INSERT) == 0, "dbm_store() on a database opened O_WRONLY failed");
	dbm_close(db);
	db = dbm_open(DBN, O_RDONLY, 0);
	pc_check(db != NULL, "dbm_open(O_RDONLY) failed");
	pc_check(dbx_has(db, dbx_str("one"), "1", 1), "the old record is gone");
	pc_check(dbx_has(db, dbx_str("two"), "22", 2), "the record stored through O_WRONLY is missing");
	dbm_close(db);
	pc_pass();
}
