/*
 * pcipc.h - helpers shared by the tests of XSH 2.7 (XSI IPC permissions).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Each case runs in a child that gives up privilege and takes chosen user
 * and group IDs (none of them used by the system's own accounts: the IDs
 * need not name accounts).
 */
#ifndef PCIPC_H
#define PCIPC_H

#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#define ID_A 30011	/* owner's user */
#define ID_B 30012	/* owner's group */
#define ID_X 30013	/* another user */
#define ID_Y 30014	/* another group */

/* A child's verdict on one operation. */
#define R_ALLOWED 0
#define R_DENIED 1	/* EACCES */
#define R_OTHER 2

static inline void ipc_on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out (a call did not return)");
}

static inline int verdict(int ok, int err)
{
	if (ok)
		return R_ALLOWED;
	return err == EACCES ? R_DENIED : R_OTHER;
}

static inline const char *verdict_name(int v)
{
	return v == R_ALLOWED ? "allowed" : v == R_DENIED ? "refused with EACCES" :
	    v == 2 ? "failed with another error" : v == -1 ? "not finished in time" :
	    v == 98 ? "could not rule out a supplementary test group" : "could not change the IDs";
}

/*
 * Run fn(arg) in a child whose real and effective IDs are u and g and which
 * has no supplementary group among the test groups. Returns fn's value
 * (0 to 2), or 98 when such a group could not be ruled out, 99 when the IDs could not be
 * changed, -1 when the child did not end in time.
 */
static inline int run_as(uid_t u, gid_t g, int (*fn)(void *), void *arg)
{
	pid_t k = fork();
	int st, i;

	if (k == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (k == 0) {
		gid_t gl[256];
		int n, j;

		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(15);
		(void)pc_drop_groups();
		if (setgid(g) != 0 || setuid(u) != 0)
			_exit(99);
		if (geteuid() != u || getegid() != g || getuid() != u || getgid() != g)
			_exit(99);
		/* A supplementary group among the test IDs (30011 to 30024) could change the class. */
		n = getgroups((int)(sizeof gl / sizeof gl[0]), gl);
		if (n < 0)
			_exit(98);
		for (j = 0; j < n; j++)
			if (gl[j] >= ID_A && gl[j] <= ID_A + 13 && gl[j] != g)
				_exit(98);
		_exit(fn(arg));
	}
	for (i = 0; i < 2000; i++) {
		pid_t r = waitpid(k, &st, WNOHANG);
		struct timespec ts = { 0, 10000000 };

		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -1;
		if (r == -1 && errno != EINTR)
			return -1;
		(void)nanosleep(&ts, NULL);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

/*
 * The classes of a mode (user, group, other) are set or not; the four kinds
 * of process are judged by one class each.
 */
enum { K_USER, K_USERGROUP, K_GROUP, K_OTHER, NKINDS };

static inline uid_t kind_uid(int kind)
{
	return (kind == K_USER || kind == K_USERGROUP) ? ID_A : ID_X;
}

static inline gid_t kind_gid(int kind)
{
	return (kind == K_USERGROUP || kind == K_GROUP) ? ID_B : ID_Y;
}

static inline const char *kind_name(int kind)
{
	return kind == K_USER ? "owner user, other group" : kind == K_USERGROUP ? "owner user and group" :
	    kind == K_GROUP ? "other user, owner group" : "other user and group";
}

/* The mode with the bits for rw in the classes selected by the three flags. */
static inline unsigned make_mode(int u, int g, int o)
{
	return (u ? 0600u : 0u) | (g ? 060u : 0u) | (o ? 06u : 0u);
}

/* Whether the class that applies to a kind of process is set in selection sel (bit 4 user, 2 group, 1 other). */
static inline int class_set(int kind, int sel)
{
	if (kind == K_USER || kind == K_USERGROUP)
		return (sel & 4) != 0;
	if (kind == K_GROUP)
		return (sel & 2) != 0;
	return (sel & 1) != 0;
}

#endif
