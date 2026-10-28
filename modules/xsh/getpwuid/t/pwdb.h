/*
 * pwdb.h - helpers for the user database tests (shared, keep identical).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef PWDB_H
#define PWDB_H

#include "pc.h"
#include <errno.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct passwd xent_t;
typedef uid_t xid_t;
#define XNAME(p) ((p)->pw_name)
#define XID(p) ((p)->pw_uid)
#define XSENTINEL EDOM

/* Errors a lookup is allowed to report (the "may fail" list of the pages). */
static inline int xc_mayfail(int e)
{
	return e == EIO || e == EINTR || e == EMFILE || e == ENFILE;
}

/* A private copy of an entry: the library's own storage is reused by later calls. */
struct xc {
	char name[512];
	xid_t id;
	gid_t gid;
	char dir[2048];
	char shell[2048];
};

static inline void xc_str(char *d, size_t n, const char *s)
{
	snprintf(d, n, "%s", s ? s : "");
}

static inline void xc_set(struct xc *c, const xent_t *p)
{
	xc_str(c->name, sizeof c->name, p->pw_name);
	c->id = p->pw_uid;
	c->gid = p->pw_gid;
	xc_str(c->dir, sizeof c->dir, p->pw_dir);
	xc_str(c->shell, sizeof c->shell, p->pw_shell);
}

static inline int xc_same(const struct xc *a, const struct xc *b)
{
	return strcmp(a->name, b->name) == 0 && a->id == b->id && a->gid == b->gid &&
	    strcmp(a->dir, b->dir) == 0 && strcmp(a->shell, b->shell) == 0;
}

static inline int xc_in(const char *s, const char *buf, size_t n)
{
	return s == NULL || ((uintptr_t)s >= (uintptr_t)buf && (uintptr_t)s < (uintptr_t)buf + n);
}

/* 1 if every pointer in the entry that refers to storage lies in buf[0..n). */
static inline int xc_inbuf(const xent_t *p, const char *buf, size_t n)
{
	return xc_in(p->pw_name, buf, n) && xc_in(p->pw_dir, buf, n) && xc_in(p->pw_shell, buf, n);
}

/* Setup: a copy of an entry that exists (the caller's, else the one of ID 0). */
static inline void xc_find(struct xc *c)
{
	uid_t cand[3];
	int i;
	struct passwd *p;

	cand[0] = getuid();
	cand[1] = geteuid();
	cand[2] = 0;
	for (i = 0; i < 3; i++) {
		p = getpwuid(cand[i]);
		if (p != NULL && p->pw_name != NULL && p->pw_name[0] != '\0') {
			xc_set(c, p);
			return;
		}
	}
	pc_unresolved("the user database has no entry for the real or effective user ID, or for 0");
}

/* Setup: an ID that no entry carries (a lookup that finds nothing; what it does to errno is for the tests to judge). */
static inline xid_t xc_absent_id(void)
{
	unsigned i;
	xid_t id;

	for (i = 0; i < 64; i++) {
		id = (xid_t)(0x7ffff000u + 17u * i);
		if (getpwuid(id) == NULL)
			return id;
	}
	pc_unresolved("no unused user ID found");
}

#endif
