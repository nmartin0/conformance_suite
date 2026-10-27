/*
 * Keys and content are arbitrary bytes of many sizes, up to a pair of 1000 bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.008
 */
#include "dbx.h"

int main(void)
{
	static const unsigned char k1[5] = { 0x00, 0xff, 0x01, 0x00, 0x7f };
	static const unsigned char c1[7] = { 0xff, 0x00, 0x00, 0x80, 0x0a, 0x00, 0x01 };
	DBM *db;
	char k2[20], c2[980];
	int i;

	pc_begin("XSH.dbm_open.008");
	dbx_init();
	for (i = 0; i < 20; i++)
		k2[i] = (char)('A' + i);
	for (i = 0; i < 980; i++)
		c2[i] = (char)((i * 37 + 11) & 0xff);
	db = dbx_open(O_RDWR | O_CREAT);
	pc_test();
	pc_check(dbm_store(db, dbx_datum(k1, 5), dbx_datum(c1, 7), DBM_INSERT) == 0, "store of binary data failed");
	pc_check(dbm_store(db, dbx_datum("x", 1), dbx_datum("y", 1), DBM_INSERT) == 0, "store of one byte key and content failed");
	pc_check(dbx_has(db, dbx_datum(k1, 5), c1, 7), "binary content was not fetched back exactly");
	pc_check(dbx_has(db, dbx_datum("x", 1), "y", 1), "one byte content was not fetched back exactly");
	pc_check(dbm_fetch(db, dbx_datum(k1, 4)).dptr == NULL, "a key that is a prefix of a stored key found content");
	dbm_close(db);
	db = dbx_open(O_RDWR | O_CREAT | O_TRUNC);
	pc_check(dbm_store(db, dbx_datum(k2, 20), dbx_datum(c2, 980), DBM_INSERT) == 0, "store of a pair of 1000 bytes failed");
	pc_check(dbx_has(db, dbx_datum(k2, 20), c2, 980), "the 980 byte content was not fetched back exactly");
	dbm_close(db);
	pc_pass();
}
