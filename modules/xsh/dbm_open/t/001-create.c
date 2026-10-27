/*
 * Creating a database makes the files the page names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.001
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	struct stat st;
	int one, two;

	pc_begin("XSH.dbm_open.001");
	dbx_init();
	pc_test();
	db = dbm_open(DBN, O_RDWR | O_CREAT, 0600);
	pc_check(db != NULL, "dbm_open(O_CREAT) returned a null pointer");
	dbm_close(db);
	one = stat(DBN ".db", &st) == 0;
	two = stat(DBN ".dir", &st) == 0 && stat(DBN ".pag", &st) == 0;
	pc_check(one || two, "neither " DBN ".db nor both " DBN ".dir and " DBN ".pag exist");
	pc_pass();
}
