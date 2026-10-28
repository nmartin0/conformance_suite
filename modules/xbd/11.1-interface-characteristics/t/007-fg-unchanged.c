/*
 * Only a controlling-terminal allocation or tcsetpgrp() changes the foreground group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.1-interface-characteristics.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void msleep(int ms)
{
	struct timespec t;

	t.tv_sec = ms / 1000;
	t.tv_nsec = (long)(ms % 1000) * 1000000L;
	while (nanosleep(&t, &t) == -1 && errno == EINTR)
		;
}

static char ptsn[256];
static int g_master = -1;

/* A manager side; ptsn gets the subsidiary name. Any problem is UNRESOLVED, absence NOTINUSE. */
static int open_manager(void)
{
	const char *name;
	int m = posix_openpt(O_RDWR | O_NOCTTY);

	if (m < 0) {
		if (errno == ENOENT || errno == ENOSYS)
			pc_notinuse("no pseudo-terminal facility: %s", pc_errname(errno));
		pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
	}
	if (grantpt(m) != 0 || unlockpt(m) != 0 || (name = ptsname(m)) == NULL)
		pc_unresolved("cannot set up the pseudo-terminal: %s", pc_errname(errno));
	if (strlen(name) >= sizeof ptsn)
		pc_unresolved("the subsidiary name is too long");
	strcpy(ptsn, name);
	g_master = m;
	return m;
}

struct rep {
	int status;
	int v[8];
	char msg[120];
};
#define RS_OK 0
#define RS_SETUP 1
#define RS_NOCTTY 2
#define RS_STOPPED 3
#define SS_CTTY 0	/* S opens the subsidiary side without O_NOCTTY and must get a controlling terminal */

static int up[2] = {-1, -1}, dn[2] = {-1, -1};
static pid_t g_sess = -1, g_leader = -1;

static void rep_set(struct rep *r, int status, const char *fmt, ...)
{
	va_list ap;

	r->status = status;
	va_start(ap, fmt);
	vsnprintf(r->msg, sizeof r->msg, fmt, ap);
	va_end(ap);
}

static void rep_send(const struct rep *r)
{
	if (write(up[1], r, sizeof *r) != (ssize_t)sizeof *r)
		_exit(2);
}

static void rep_ok(void)
{
	struct rep r;

	memset(&r, 0, sizeof r);
	rep_send(&r);
}

/* In S: the next byte T sent, or -1 after ms. */
static int dn_wait(int ms)
{
	struct pollfd pf;
	char c;
	int n;

	pf.fd = dn[0];
	pf.events = POLLIN;
	pf.revents = 0;
	do
		n = poll(&pf, 1, ms);
	while (n == -1 && errno == EINTR);
	if (n <= 0 || read(dn[0], &c, 1) != 1)
		return -1;
	return (unsigned char)c;
}

static void dn_send(char c)
{
	NEED(write(dn[1], &c, 1) == 1, "cannot signal the session leader: %s", pc_errname(errno));
}

static void reap_session(void)
{
	if (g_sess > 0) {
		kill(g_sess, SIGKILL);
		waitpid(g_sess, NULL, 0);
		g_sess = -1;
	}
}

/* In T: the next report from S (or its children), waiting ms. */
static void rep_get(struct rep *r, int ms)
{
	struct pollfd pf;
	int n;

	pf.fd = up[0];
	pf.events = POLLIN;
	pf.revents = 0;
	do
		n = poll(&pf, 1, ms);
	while (n == -1 && errno == EINTR);
	if (n <= 0 || read(up[0], r, sizeof *r) != (ssize_t)sizeof *r) {
		reap_session();
		pc_unresolved("the session leader did not report");
	}
}

/* Harness problems of a report are UNRESOLVED; a stop that was not expected is a FAIL. */
static void settle(const struct rep *r)
{
	if (r->status == RS_OK)
		return;
	if (r->status == RS_STOPPED)
		pc_fail("%s", r->msg);
	pc_unresolved("%s", r->msg);
}

typedef void (*sbodyfn)(int slave);

/*
 * T forks a session leader S: setsid(), then (SS_CTTY) opens the subsidiary side without O_NOCTTY
 * and checks that it became the controlling terminal (how one is acquired is implementation-
 * defined, so a refusal is UNRESOLVED). S reports when ready, waits for T's go byte, runs body
 * and then waits for T's end byte. g_master (if open) is closed in S.
 */
static void start_session(int how, sbodyfn body)
{
	static int once;
	struct rep r;
	pid_t s;

	if (!once) {
		atexit(reap_session);
		signal(SIGPIPE, SIG_IGN);
		once = 1;
	}
	NEED(pipe(up) == 0 && pipe(dn) == 0, "pipe failed: %s", pc_errname(errno));
	fflush(NULL);
	s = fork();
	NEED(s != -1, "fork failed: %s", pc_errname(errno));
	if (s == 0) {
		int slave = -1;

		close(up[0]);
		close(dn[1]);
		if (g_master >= 0)
			close(g_master);
		g_sess = -1;
		alarm(50);
		memset(&r, 0, sizeof r);
		if (setsid() == -1) {
			rep_set(&r, RS_SETUP, "setsid failed: %s", pc_errname(errno));
			rep_send(&r);
			_exit(0);
		}
		if (how == SS_CTTY) {
			slave = open(ptsn, O_RDWR);
			if (slave < 0) {
				rep_set(&r, RS_SETUP, "cannot open %s: %s", ptsn, pc_errname(errno));
				rep_send(&r);
				_exit(0);
			}
			if (tcgetpgrp(slave) != getpgrp()) {
				rep_set(&r, RS_NOCTTY, "opening the subsidiary side did not make it the controlling terminal (acquisition is implementation-defined)");
				rep_send(&r);
				_exit(0);
			}
		}
		rep_ok();
		if (dn_wait(40000) != 'g')
			_exit(1);
		body(slave);
		(void)dn_wait(40000);
		_exit(0);
	}
	g_sess = g_leader = s;
	close(up[1]);
	close(dn[0]);
	rep_get(&r, 30000);
	settle(&r);
}

static void go(void)
{
	dn_send('g');
}

static void end_session(void)
{
	char c = 'q';

	if (g_sess > 0) {
		(void)!write(dn[1], &c, 1);
		waitpid(g_sess, NULL, 0);
		g_sess = -1;
	}
	close(up[0]);
	close(dn[1]);
}


static void sb(int slave)
{
	struct rep r;
	pid_t c1, c2, c3;
	pid_t fg0 = tcgetpgrp(slave);
	int hold[2], i;
	char x;

	memset(&r, 0, sizeof r);
	if (pipe(hold) != 0) {
		rep_set(&r, RS_SETUP, "pipe failed");
		rep_send(&r);
		return;
	}
	c1 = fork();
	if (c1 == 0) {
		close(hold[1]);
		(void)setpgid(0, 0);
		alarm(15);
		(void)!read(hold[0], &x, 1);
		_exit(0);
	}
	if (c1 == -1) {
		rep_set(&r, RS_SETUP, "fork failed");
		rep_send(&r);
		return;
	}
	(void)setpgid(c1, c1);
	for (i = 0; i < 100 && getpgid(c1) != c1; i++)
		msleep(10);
	r.v[0] = (int)tcgetpgrp(slave);
	c2 = fork();
	if (c2 == 0) {
		close(hold[1]);
		alarm(15);
		(void)!read(hold[0], &x, 1);
		_exit(0);
	}
	(void)setpgid(c2, c1);
	r.v[1] = (int)tcgetpgrp(slave);
	c3 = fork();
	if (c3 == 0) {
		(void)setsid();
		_exit(0);
	}
	if (c3 > 0)
		waitpid(c3, NULL, 0);
	r.v[2] = (int)tcgetpgrp(slave);
	close(hold[1]);
	if (c1 > 0)
		waitpid(c1, NULL, 0);
	if (c2 > 0)
		waitpid(c2, NULL, 0);
	r.v[3] = (int)tcgetpgrp(slave);
	r.v[4] = (int)fg0;
	rep_send(&r);
}

int main(void)
{
	struct rep r;
	int m, i;

	pc_begin("XBD.11.1-interface-characteristics.007");
	m = open_manager();
	(void)m;
	start_session(SS_CTTY, sb);
	pc_test();
	go();
	rep_get(&r, 30000);
	end_session();
	settle(&r);
	pc_check(r.v[4] == (int)g_leader, "the foreground group at the start was %d, not the leader's %d", r.v[4], (int)g_leader);
	for (i = 0; i < 4; i++)
		pc_check(r.v[i] == r.v[4], "after step %d the foreground group was %d instead of %d", i, r.v[i], r.v[4]);
	pc_pass();
}
