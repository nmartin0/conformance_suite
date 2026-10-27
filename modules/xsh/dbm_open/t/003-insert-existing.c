/*
 * DBM_INSERT on an existing key returns 1 and keeps the old record.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.003
 */
#include "dbx.h"

int main(void)
{
	DBM *db;

	pc_begin("XSH.dbm_open.003");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_check(dbm_store(db, dbx_str("k"), dbx_str("old"), DBM_INSERT) == 0, "first store failed");
	pc_test();
	pc_check(dbm_store(db, dbx_str("k"), dbx_str("brand new"), DBM_INSERT) == 1, "DBM_INSERT of an existing key did not return 1");
	pc_check(dbx_has(db, dbx_str("k"), "old", 3), "the existing record was changed by DBM_INSERT");
	dbm_close(db);
	pc_pass();
}
