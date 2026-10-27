/*
 * utx.h - helpers for the user accounting database tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef UTX_H
#define UTX_H

#include "pc.h"
#include <string.h>
#include <utmpx.h>

#define UTX_MAX 256	/* entries kept for the later searches */
#define UTX_CAP 1000000	/* entries read before giving up */

/* Nothing is assumed about the database: it may be absent, empty or hidden. */
static inline int utx_known_type(int t)
{
	return t == EMPTY || t == BOOT_TIME || t == OLD_TIME || t == NEW_TIME ||
	    t == USER_PROCESS || t == INIT_PROCESS || t == LOGIN_PROCESS || t == DEAD_PROCESS;
}

static inline int utx_proc_type(int t)
{
	return t == USER_PROCESS || t == INIT_PROCESS || t == LOGIN_PROCESS || t == DEAD_PROCESS;
}

static inline int utx_same(const struct utmpx *a, const struct utmpx *b)
{
	return a->ut_type == b->ut_type && a->ut_pid == b->ut_pid &&
	    memcmp(a->ut_id, b->ut_id, sizeof a->ut_id) == 0 &&
	    memcmp(a->ut_line, b->ut_line, sizeof a->ut_line) == 0;
}

/* Read from the start to the end: copies of the first UTX_MAX entries go to e; returns how many were read. */
static inline long utx_read_all(struct utmpx *e, long *kept)
{
	struct utmpx *p;
	long n = 0;

	*kept = 0;
	setutxent();
	while ((p = getutxent()) != NULL) {
		if (n < UTX_MAX) {
			e[n] = *p;
			*kept = n + 1;
		}
		if (++n > UTX_CAP)
			pc_unresolved("more than %d entries", UTX_CAP);
	}
	return n;
}

#endif
