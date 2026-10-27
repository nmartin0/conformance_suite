/*
 * With no process left in the foreground group, tcgetpgrp() gives an unused value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcgetpgrp.005
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
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static char ptsn[256];

/* A manager side; ptsn gets the subsidiary device's name. Any problem is UNRESOLVED. */
static int open_manager(void)
{
	const char *name;
	int m = posix_openpt(O_RDWR | O_NOCTTY);

	if (m < 0)
		pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
	if (grantpt(m) != 0 || unlockpt(m) != 0 || (name = ptsname(m)) == NULL)
		pc_unresolved("cannot set up the pseudo-terminal: %s", pc_errname(errno));
	if (strlen(name) >= sizeof ptsn)
		pc_unresolved("the subsidiary name is too long");
	strcpy(ptsn, name);
	return m;
}

/* The subsidiary side opened without becoming a controlling terminal. */
static int open_pty(int *m)
{
	int s;

	*m = open_manager();
	s = open(ptsn, O_RDWR | O_NOCTTY);
	if (s < 0)
		pc_unresolved("cannot open %s: %s", ptsn, pc_errname(errno));
	return s;
}

/*
 * A session of its own with a pseudo-terminal. T (this process) makes a
 * session leader S; S calls setsid(), opens the subsidiary side (without
 * O_NOCTTY unless asked) and, in the modes that need it, checks that it
 * became the controlling terminal (how it is acquired is implementation-
 * defined, so a refusal is UNRESOLVED). M_BG then runs the body in a child
 * A of S in its own process group: not orphaned, in the background. The
 * body fills a struct that is sent back over a pipe. Every process has an
 * alarm; S waits for T to finish before it exits.
 */
struct rep {
	int status;
	int v[8];
	char msg[120];
};
#define RS_OK 0
#define RS_SETUP 1
#define RS_NOCTTY 2
#define RS_STOPPED 3
#define RS_NOTINUSE 4
#define M_SELF 0
#define M_BG 1
#define M_NOCTTY 2

typedef void (*bodyfn)(int slave, struct rep *r);
typedef void (*prepfn)(int slave, pid_t apid, struct rep *r);

static int rep_pipe[2], done_pipe[2];

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
	if (write(rep_pipe[1], r, sizeof *r) != (ssize_t)sizeof *r)
		_exit(2);
}

/* A second pseudo-terminal's subsidiary side, not a controlling terminal; -1 on any problem. */
static int open_pty2(void)
{
	const char *n;
	int m2 = posix_openpt(O_RDWR | O_NOCTTY);

	if (m2 < 0)
		return -1;
	if (grantpt(m2) != 0 || unlockpt(m2) != 0 || (n = ptsname(m2)) == NULL)
		return -1;
	return open(n, O_RDWR | O_NOCTTY);
}

/* A child in a process group of its own that lives until *release is closed; returns its pid or -1. */
static pid_t mk_member(int *release)
{
	int p[2];
	pid_t c;

	if (pipe(p) != 0)
		return -1;
	c = fork();
	if (c == -1)
		return -1;
	if (c == 0) {
		char b;

		close(p[1]);
		alarm(20);
		(void)setpgid(0, 0);
		if (read(p[0], &b, 1) < 0)
			_exit(1);
		_exit(0);
	}
	close(p[0]);
	(void)setpgid(c, c);
	*release = p[1];
	return c;
}

static int acquired(int slave, int by_tcgetpgrp)
{
	char cb[L_ctermid];
	int fd;

	if (by_tcgetpgrp)
		return tcgetpgrp(slave) == getpgrp();
	ctermid(cb);
	if (cb[0] == '\0')
		return 0;
	fd = open(cb, O_RDWR | O_NOCTTY);
	if (fd < 0)
		return 0;
	close(fd);
	return 1;
}

/* Runs body in a background child A of S; returns 1 if A sent the report. */
static int bg_run(int slave, bodyfn body, prepfn prep, struct rep *r)
{
	int go[2], st;
	pid_t a;

	if (pipe(go) != 0) {
		rep_set(r, RS_SETUP, "pipe failed");
		return 0;
	}
	a = fork();
	if (a == -1) {
		rep_set(r, RS_SETUP, "fork failed");
		return 0;
	}
	if (a == 0) {
		char c;

		close(go[1]);
		alarm(20);
		if (setpgid(0, 0) != 0) {
			rep_set(r, RS_SETUP, "setpgid failed: %s", pc_errname(errno));
			rep_send(r);
			_exit(0);
		}
		if (read(go[0], &c, 1) != 1)
			_exit(1);
		body(slave, r);
		rep_send(r);
		_exit(0);
	}
	close(go[0]);
	(void)setpgid(a, a);
	if (prep != NULL) {
		prep(slave, a, r);
		if (r->status != RS_OK) {
			kill(a, SIGKILL);
			waitpid(a, &st, 0);
			return 0;
		}
	}
	if (write(go[1], "g", 1) != 1) {
		kill(a, SIGKILL);
		waitpid(a, &st, 0);
		rep_set(r, RS_SETUP, "write failed");
		return 0;
	}
	if (waitpid(a, &st, WUNTRACED) == -1) {
		rep_set(r, RS_SETUP, "waitpid failed");
		return 0;
	}
	if (WIFSTOPPED(st)) {
		rep_set(r, RS_STOPPED, "the caller was stopped by signal %d", WSTOPSIG(st));
		kill(a, SIGKILL);
		waitpid(a, &st, 0);
		return 0;
	}
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0) {
		rep_set(r, RS_SETUP, "the caller ended abnormally (status %#x)", (unsigned)st);
		return 0;
	}
	return 1;
}

static void leader(int mode, int by_tcgetpgrp, bodyfn body, prepfn prep)
{
	struct rep r;
	int slave, sent = 0;
	char c;

	memset(&r, 0, sizeof r);
	signal(SIGPIPE, SIG_IGN);
	close(rep_pipe[0]);
	close(done_pipe[1]);
	alarm(25);
	if (setsid() == -1) {
		rep_set(&r, RS_SETUP, "setsid failed: %s", pc_errname(errno));
	} else if ((slave = open(ptsn, mode == M_NOCTTY ? O_RDWR | O_NOCTTY : O_RDWR)) < 0) {
		rep_set(&r, RS_SETUP, "cannot open %s: %s", ptsn, pc_errname(errno));
	} else if (mode != M_NOCTTY && !acquired(slave, by_tcgetpgrp)) {
		rep_set(&r, RS_NOCTTY, "opening the subsidiary side did not make it the controlling terminal (acquisition is implementation-defined)");
	} else if (mode == M_BG) {
		sent = bg_run(slave, body, prep, &r);
	} else {
		body(slave, &r);
	}
	if (!sent)
		rep_send(&r);
	if (read(done_pipe[0], &c, 1) < 0)
		_exit(1);
	_exit(0);
}

/* In T: starts the session, waits (30 s at most) for the report. */
static struct rep run_session(int mode, int by_tcgetpgrp, bodyfn body, prepfn prep, pid_t *spid)
{
	struct rep r;
	struct pollfd pf;
	pid_t s;
	int n;

	if (pipe(rep_pipe) != 0 || pipe(done_pipe) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	fflush(NULL);
	s = fork();
	if (s == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (s == 0) {
		leader(mode, by_tcgetpgrp, body, prep);
		_exit(0);
	}
	close(rep_pipe[1]);
	close(done_pipe[0]);
	pf.fd = rep_pipe[0];
	pf.events = POLLIN;
	do
		n = poll(&pf, 1, 30000);
	while (n == -1 && errno == EINTR);
	if (n <= 0 || read(rep_pipe[0], &r, sizeof r) != (ssize_t)sizeof r) {
		kill(s, SIGKILL);
		waitpid(s, NULL, 0);
		pc_unresolved("the session leader did not report in 30 s");
	}
	*spid = s;
	return r;
}

static void end_session(pid_t s)
{
	close(done_pipe[1]);
	waitpid(s, NULL, 0);
	close(rep_pipe[0]);
}

/* Reports problems of the harness; returns only for RS_OK. */
static void settle(const struct rep *r)
{
	if (r->status == RS_OK)
		return;
	if (r->status == RS_STOPPED)
		pc_fail("%s", r->msg);
	if (r->status == RS_NOTINUSE)
		pc_notinuse("%s", r->msg);
	pc_unresolved("%s", r->msg);
}


static void body(int slave, struct rep *r)
{
	int rel, st;
	pid_t b, g;

	signal(SIGTTOU, SIG_IGN);
	b = mk_member(&rel);
	if (b < 0) {
		rep_set(r, RS_SETUP, "cannot create a process group");
		return;
	}
	if (tcsetpgrp(slave, b) != 0) {
		rep_set(r, RS_SETUP, "set-up tcsetpgrp() failed: %s", pc_errname(errno));
		close(rel);
		return;
	}
	close(rel);
	if (waitpid(b, &st, 0) != b) {
		rep_set(r, RS_SETUP, "waitpid failed");
		return;
	}
	g = tcgetpgrp(slave);
	r->v[0] = (int)g;
	r->v[1] = errno;
	r->v[4] = (int)getpgrp();
	if (g > 1) {
		errno = 0;
		r->v[2] = kill(-g, 0);
		r->v[3] = errno;
	}
}

int main(void)
{
	struct rep rep;
	pid_t s;

	pc_begin("XSH.tcgetpgrp.005");
	(void)open_manager();
	pc_test();
	rep = run_session(M_SELF, 0, body, NULL, &s);
	end_session(s);
	settle(&rep);
	pc_check(rep.v[0] > 1, "tcgetpgrp() returned %d (errno %s), expected a value greater than 1", rep.v[0], pc_errname(rep.v[1]));
	pc_check(rep.v[0] != rep.v[4], "tcgetpgrp() returned the group %d of a live process", rep.v[0]);
	pc_check(rep.v[2] == -1 && rep.v[3] == ESRCH, "the value %d is the ID of an existing process group (kill gave %d, errno %s)", rep.v[0],
		 rep.v[2], pc_errname(rep.v[3]));
	pc_pass();
}
