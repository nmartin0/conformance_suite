/*
 * With O_CLOEXEC the descriptors the database opens have FD_CLOEXEC set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dbm_open.013
 */
#include "dbx.h"

#define NFD 256

static void check_new(const int *before, const char *what)
{
	int fd, found = 0, f;

	for (fd = 0; fd < NFD; fd++) {
		if (before[fd])
			continue;
		f = fcntl(fd, F_GETFD);
		if (f == -1)
			continue;
		found++;
		pc_check((f & FD_CLOEXEC) != 0, "descriptor %d opened by %s has no FD_CLOEXEC", fd, what);
	}
	if (found == 0)
		pc_unresolved("%s opened no descriptor that could be seen", what);
}

int main(void)
{
	int before[NFD], fd;
	DBM *db;

	pc_begin("XSH.dbm_open.013");
	dbx_init();
	pc_test();
	for (fd = 0; fd < NFD; fd++)
		before[fd] = fcntl(fd, F_GETFD) != -1;
	db = dbm_open(DBN, O_RDWR | O_CREAT | O_CLOEXEC, 0600);
	pc_check(db != NULL, "dbm_open(O_CREAT | O_CLOEXEC) failed");
	check_new(before, "creating");
	dbm_close(db);
	for (fd = 0; fd < NFD; fd++)
		before[fd] = fcntl(fd, F_GETFD) != -1;
	db = dbm_open(DBN, O_RDWR | O_CLOEXEC, 0);
	pc_check(db != NULL, "dbm_open(O_CLOEXEC) of an existing database failed");
	check_new(before, "opening");
	dbm_close(db);
	pc_pass();
}
