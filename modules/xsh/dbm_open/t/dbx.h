/*
 * dbx.h - helpers for the database (ndbm) tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The database is called pcdb in the test's private directory. The names of
 * the underlying files are only known up to a suffix, so the cleanup removes
 * every file there whose name begins with pcdb, on every way out of the test.
 */
#ifndef DBX_H
#define DBX_H

#include "pc.h"
#include <dirent.h>
#include <fcntl.h>
#include <ndbm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DBN "pcdb"

static inline void dbx_clean(void)
{
	DIR *d;
	struct dirent *e;
	char names[64][64];
	int n = 0, i;

	d = opendir(".");
	if (d == NULL)
		return;
	while ((e = readdir(d)) != NULL && n < 64)
		if (strncmp(e->d_name, DBN, strlen(DBN)) == 0 && strlen(e->d_name) < 64)
			strcpy(names[n++], e->d_name);
	closedir(d);
	for (i = 0; i < n; i++)
		(void)unlink(names[i]);
}

/* Call first in setup: removes leftovers and arranges the removal at exit. */
static inline void dbx_init(void)
{
	dbx_clean();
	if (atexit(dbx_clean) != 0)
		pc_unresolved("atexit failed");
}

static inline datum dbx_datum(const void *p, size_t n)
{
	datum d;

	memset(&d, 0, sizeof d);
	d.dptr = (void *)p;
	d.dsize = n;
	return d;
}

static inline datum dbx_str(const char *s)
{
	return dbx_datum(s, strlen(s));
}

/* 1 if the database holds exactly the content (p, n) for the key. */
static inline int dbx_has(DBM *db, datum key, const void *p, size_t n)
{
	datum c = dbm_fetch(db, key);

	return c.dptr != NULL && (size_t)c.dsize == n && (n == 0 || memcmp(c.dptr, p, n) == 0);
}

/* Setup: open (or create) the database read/write. */
static inline DBM *dbx_open(int flags)
{
	DBM *db = dbm_open(DBN, flags, 0600);

	pc_check(db != NULL, "dbm_open failed");
	return db;
}

static inline void dbx_key(char *buf, size_t n, int i)
{
	snprintf(buf, n, "key-%04d", i);
}

static inline void dbx_val(char *buf, size_t n, int i)
{
	snprintf(buf, n, "value-%04d-abcdefghijklmnopqrstuvwxyz", i);
}

/* Counts the keys of the database by firstkey/nextkey; marks seen[i] for the keys of the form key-%04d. */
static inline long dbx_walk(DBM *db, int *seen, int nseen, long *dups, long *others)
{
	datum k;
	long n = 0;
	char buf[32];
	int i;

	*dups = 0;
	*others = 0;
	for (k = dbm_firstkey(db); k.dptr != NULL; k = dbm_nextkey(db)) {
		if (++n > 100000)
			pc_fail("the key iteration does not end");
		if (k.dsize == 8 && memcmp(k.dptr, "key-", 4) == 0) {
			memcpy(buf, k.dptr, 8);
			buf[8] = '\0';
			i = atoi(buf + 4);
			if (i >= 0 && i < nseen) {
				if (seen[i]++)
					(*dups)++;
				continue;
			}
		}
		(*others)++;
	}
	return n;
}

#endif
