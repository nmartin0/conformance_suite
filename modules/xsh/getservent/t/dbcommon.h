/*
 * dbcommon.h - sequence, descriptor and rewind helpers for the database tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The including file's db.h defines DBT (the entry type), GETENT(),
 * SETENT(n), ENDENT(), sig_of() and shape().
 */
#ifndef DBCOMMON_H
#define DBCOMMON_H
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "db.h"

#define CAP 20000
#define SIGMAX 2048
#define FDMAX 256

static char *seq[2][CAP];
static int seqn[2];

/* Read every entry from the first with SETENT(stayopen); the signatures go to seq[which]. */
static void collect(int which, int stayopen, int check_shape)
{
	int n = 0;

	SETENT(stayopen);
	for (;;) {
		const DBT *e = GETENT();
		char buf[SIGMAX];

		if (e == NULL)
			break;
		if (n >= CAP)
			pc_unresolved("the database has more than %d entries", CAP);
		if (check_shape)
			shape(e);
		sig_of(e, buf, sizeof buf);
		seq[which][n] = strdup(buf);
		if (seq[which][n] == NULL)
			pc_unresolved("out of memory");
		n++;
	}
	seqn[which] = n;
}

static void same_sequences(void)
{
	int i;

	pc_check(seqn[0] == seqn[1], "the first pass gave %d entries, the second %d", seqn[0], seqn[1]);
	for (i = 0; i < seqn[0]; i++)
		pc_check(strcmp(seq[0][i], seq[1][i]) == 0, "entry %d differs between passes: \"%s\" and \"%s\"", i, seq[0][i], seq[1][i]);
}

static void snapshot(unsigned char open_[FDMAX])
{
	int fd;

	for (fd = 0; fd < FDMAX; fd++)
		open_[fd] = fcntl(fd, F_GETFD) != -1;
}
#endif
