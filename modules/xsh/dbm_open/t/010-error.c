/*
 * Without errors the error condition is clear, also after the end of the key iteration and dbm_clearerr().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.010
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	datum k;

	pc_begin("XSH.dbm_open.010");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_test();
	pc_check(dbm_error(db) == 0, "the error condition is set on a new database");
	k = dbm_firstkey(db);
	pc_check(k.dptr == NULL, "dbm_firstkey() of an empty database returned a key");
	pc_check(dbm_error(db) == 0, "the error condition is set after the end of an empty iteration");
	pc_check(dbm_store(db, dbx_str("a"), dbx_str("b"), DBM_INSERT) == 0, "store failed");
	(void)dbm_fetch(db, dbx_str("a"));
	pc_check(dbm_error(db) == 0, "the error condition is set after normal calls");
	(void)dbm_clearerr(db);
	pc_check(dbm_error(db) == 0, "the error condition is set after dbm_clearerr()");
	dbm_close(db);
	pc_pass();
}
