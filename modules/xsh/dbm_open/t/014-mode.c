/*
 * The files of a new database get the requested mode and the effective user ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.014
 */
#include "dbx.h"

int main(void)
{
	DBM *db;
	DIR *d;
	struct dirent *e;
	struct stat st;
	int n = 0;

	pc_begin("XSH.dbm_open.014");
	dbx_init();
	(void)umask(0);
	pc_test();
	db = dbm_open(DBN, O_RDWR | O_CREAT, 0640);
	pc_check(db != NULL, "dbm_open(O_CREAT) failed");
	pc_check(dbm_store(db, dbx_str("a"), dbx_str("b"), DBM_INSERT) == 0, "store failed");
	dbm_close(db);
	d = opendir(".");
	pc_check(d != NULL, "opendir failed");
	while ((e = readdir(d)) != NULL) {
		if (strncmp(e->d_name, DBN, strlen(DBN)) != 0)
			continue;
		pc_check(stat(e->d_name, &st) == 0, "stat of %s failed", e->d_name);
		pc_check((st.st_mode & 07777) == 0640, "%s has mode %o, not 640", e->d_name, (unsigned)(st.st_mode & 07777));
		pc_check(st.st_uid == geteuid(), "%s belongs to user %ld, not the effective user", e->d_name, (long)st.st_uid);
		n++;
	}
	closedir(d);
	pc_check(n > 0, "no file of the database was found");
	pc_pass();
}
