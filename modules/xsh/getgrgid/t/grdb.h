/*
 * grdb.h - helpers for the group database tests (shared, keep identical).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef GRDB_H
#define GRDB_H

#include "pc.h"
#include <errno.h>
#include <grp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct group xent_t;
typedef gid_t xid_t;
#define XNAME(p) ((p)->gr_name)
#define XID(p) ((p)->gr_gid)
#define XSENTINEL EDOM

/* Errors a lookup is allowed to report (the "may fail" list of the pages). */
static inline int xc_mayfail(int e)
{
	return e == EIO || e == EINTR || e == EMFILE || e == ENFILE;
}
#define XMEMMAX 32

/* A private copy of an entry: the library's own storage is reused by later calls. */
struct xc {
	char name[512];
	xid_t id;
	long nmem;		/* members counted, may exceed XMEMMAX */
	char mem[XMEMMAX][128];	/* the first XMEMMAX names */
};

static inline void xc_str(char *d, size_t n, const char *s)
{
	snprintf(d, n, "%s", s ? s : "");
}

static inline void xc_set(struct xc *c, const xent_t *p)
{
	long i;

	xc_str(c->name, sizeof c->name, p->gr_name);
	c->id = p->gr_gid;
	c->nmem = 0;
	if (p->gr_mem == NULL)
		return;
	for (i = 0; p->gr_mem[i] != NULL && i < 100000; i++)
		if (i < XMEMMAX)
			xc_str(c->mem[i], sizeof c->mem[i], p->gr_mem[i]);
	c->nmem = i;
}

static inline int xc_same(const struct xc *a, const struct xc *b)
{
	long i;

	if (strcmp(a->name, b->name) != 0 || a->id != b->id || a->nmem != b->nmem)
		return 0;
	for (i = 0; i < a->nmem && i < XMEMMAX; i++)
		if (strcmp(a->mem[i], b->mem[i]) != 0)
			return 0;
	return 1;
}

static inline int xc_in(const void *s, const char *buf, size_t n)
{
	return s == NULL || ((uintptr_t)s >= (uintptr_t)buf && (uintptr_t)s < (uintptr_t)buf + n);
}

/* 1 if every pointer in the entry that refers to storage lies in buf[0..n). */
static inline int xc_inbuf(const xent_t *p, const char *buf, size_t n)
{
	long i;

	if (!xc_in(p->gr_name, buf, n) || !xc_in(p->gr_mem, buf, n))
		return 0;
	for (i = 0; p->gr_mem != NULL && p->gr_mem[i] != NULL && i < 100000; i++)
		if (!xc_in(p->gr_mem[i], buf, n))
			return 0;
	return 1;
}

/* Setup: a copy of an entry that exists (the caller's group, else the one of ID 0). */
static inline void xc_find(struct xc *c)
{
	gid_t cand[3];
	int i;
	struct group *p;

	cand[0] = getgid();
	cand[1] = getegid();
	cand[2] = 0;
	for (i = 0; i < 3; i++) {
		p = getgrgid(cand[i]);
		if (p != NULL && p->gr_name != NULL && p->gr_name[0] != '\0') {
			xc_set(c, p);
			return;
		}
	}
	pc_unresolved("the group database has no entry for the real or effective group ID, or for 0");
}

/* Setup: an ID that no entry carries (a lookup that finds nothing; what it does to errno is for the tests to judge). */
static inline xid_t xc_absent_id(void)
{
	unsigned i;
	xid_t id;

	for (i = 0; i < 64; i++) {
		id = (xid_t)(0x7ffff000u + 17u * i);
		if (getgrgid(id) == NULL)
			return id;
	}
	pc_unresolved("no unused group ID found");
}

#endif
