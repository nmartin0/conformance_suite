/*
 * O_CREAT with O_EXCL fails when the database exists; without O_CREAT a missing database is not created.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.012
 */
#include "dbx.h"

int main(void)
{
	DBM *db;

	pc_begin("XSH.dbm_open.012");
	dbx_init();
	pc_test();
	db = dbm_open(DBN, O_RDWR, 0600);
	pc_check(db == NULL, "dbm_open() of a database that does not exist, without O_CREAT, succeeded");
	db = dbm_open(DBN, O_RDWR | O_CREAT | O_EXCL, 0600);
	pc_check(db != NULL, "dbm_open(O_CREAT | O_EXCL) of a new database failed");
	dbm_close(db);
	db = dbm_open(DBN, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (db != NULL)
		dbm_close(db);
	pc_check(db == NULL, "dbm_open(O_CREAT | O_EXCL) of an existing database succeeded");
	db = dbm_open(DBN, O_RDWR | O_CREAT, 0600);
	pc_check(db != NULL, "dbm_open(O_CREAT) of an existing database failed");
	dbm_close(db);
	pc_pass();
}
