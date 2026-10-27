/*
 * Fetching a key that is not stored gives a null dptr.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.005
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	datum d;

	pc_begin("XSH.dbm_open.005");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_test();
	d = dbm_fetch(db, dbx_str("nothing"));
	pc_check(d.dptr == NULL, "dbm_fetch() on an empty database returned content");
	pc_check(dbm_store(db, dbx_str("something"), dbx_str("v"), DBM_INSERT) == 0, "store failed");
	d = dbm_fetch(db, dbx_str("nothing"));
	pc_check(d.dptr == NULL, "dbm_fetch() of an absent key returned content");
	d = dbm_fetch(db, dbx_str("somethin"));
	pc_check(d.dptr == NULL, "dbm_fetch() of a shorter key returned content");
	dbm_close(db);
	pc_pass();
}
