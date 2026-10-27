/*
 * DBM_REPLACE replaces an existing record and inserts a new one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.004
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	int seen[2] = { 0, 0 };
	long dups, others, n;

	pc_begin("XSH.dbm_open.004");
	dbx_init();
	db = dbx_open(O_RDWR | O_CREAT);
	pc_check(dbm_store(db, dbx_str("key-0000"), dbx_str("old"), DBM_INSERT) == 0, "first store failed");
	pc_test();
	pc_check(dbm_store(db, dbx_str("key-0000"), dbx_str("a longer replacement"), DBM_REPLACE) == 0, "DBM_REPLACE of an existing key did not return 0");
	pc_check(dbx_has(db, dbx_str("key-0000"), "a longer replacement", 20), "the record was not replaced");
	pc_check(dbm_store(db, dbx_str("key-0001"), dbx_str("fresh"), DBM_REPLACE) == 0, "DBM_REPLACE of a new key did not return 0");
	pc_check(dbx_has(db, dbx_str("key-0001"), "fresh", 5), "the new record was not inserted by DBM_REPLACE");
	n = dbx_walk(db, seen, 2, &dups, &others);
	pc_check(n == 2 && seen[0] == 1 && seen[1] == 1 && dups == 0 && others == 0, "the database does not hold exactly the two keys (%ld keys)", n);
	dbm_close(db);
	pc_pass();
}
