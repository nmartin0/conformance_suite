/*
 * A record stored in a new database is fetched back exactly.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.002
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	datum d;

	pc_begin("XSH.dbm_open.002");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_test();
	pc_check(dbm_store(db, dbx_str("alpha"), dbx_str("first value"), DBM_INSERT) == 0, "dbm_store(DBM_INSERT) of a new key did not return 0");
	d = dbm_fetch(db, dbx_str("alpha"));
	pc_check(d.dptr != NULL, "dbm_fetch() found nothing for a stored key");
	pc_check(d.dsize == 11 && memcmp(d.dptr, "first value", 11) == 0, "dbm_fetch() returned other content");
	dbm_close(db);
	pc_pass();
}
