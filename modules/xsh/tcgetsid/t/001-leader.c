/*
 * tcgetsid() in the session leader.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcgetsid.001
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

/* 1 if fd becomes readable within ms. */
static int wait_in(int fd, int ms)
{
	struct pollfd pf;
	int n;

	pf.fd = fd;
	pf.events = POLLIN;
	pf.revents = 0;
	do
		n = poll(&pf, 1, ms);
	while (n == -1 && errno == EINTR);
	return n > 0 && (pf.revents & POLLIN) != 0;
}

/* Reads what arrives: waits ms for the first byte, then 50 ms of quiet; returns the count. */
static int collect(int fd, char *b, int cap, int ms)
{
	int n = 0;

	while (n < cap && wait_in(fd, n == 0 ? ms : 50)) {
		ssize_t r = read(fd, b + n, (size_t)(cap - n));

		if (r <= 0)
			break;
		n += (int)r;
	}
	return n;
}


static char ptsn[256];

/*
 * A manager side; nm gets the subsidiary device's name. Any problem is
 * UNRESOLVED; the absence of the facility is NOTINUSE.
 */
static int open_manager_n(char *nm, size_t sz)
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
	if (strlen(name) >= sz)
		pc_unresolved("the subsidiary name is too long");
	strcpy(nm, name);
	return m;
}

static int open_manager(void)
{
	return open_manager_n(ptsn, sizeof ptsn);
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

/* Non-canonical, no echo, no output processing, no signal characters; VMIN and VTIME as given. */
static void raw_mode(int s, int vmin, int vtime)
{
	struct termios t;

	if (tcgetattr(s, &t) != 0)
		pc_unresolved("tcgetattr failed: %s", pc_errname(errno));
	t.c_iflag &= ~(tcflag_t)(ICRNL | INLCR | IGNCR | IXON | IXOFF);
	t.c_oflag &= ~(tcflag_t)OPOST;
	t.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ISIG | IEXTEN);
	t.c_cc[VMIN] = (cc_t)vmin;
	t.c_cc[VTIME] = (cc_t)vtime;
	if (tcsetattr(s, TCSANOW, &t) != 0)
		pc_unresolved("tcsetattr failed: %s", pc_errname(errno));
}

/*
 * A session of its own with a pseudo-terminal. T (this process) makes a
 * session leader S; S calls setsid(), opens the subsidiary side (without
 * O_NOCTTY unless asked) and checks that it became the controlling terminal
 * (how it is acquired is implementation-defined, so a refusal is
 * UNRESOLVED). The body then runs in S (M_FG), in a child A of S in its own
 * process group, not orphaned (M_BG), in a process whose group has become
 * orphaned (M_ORPH), or in S without a controlling terminal (M_NOCTTY).
 * The body fills a struct that is sent back over a pipe. Every process has
 * an alarm; S waits for T to finish before it exits.
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
#define M_FG 0
#define M_BG 1
#define M_ORPH 2
#define M_NOCTTY 3

typedef void (*bodyfn)(int slave, struct rep *r);

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

/* 1 if the process has a controlling terminal (the pathname from ctermid() can be opened). */
static int acquired(void)
{
	char cb[L_ctermid];
	int fd;

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
static int bg_run(int slave, bodyfn body, struct rep *r)
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
		r->v[0] = WSTOPSIG(st);
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

/*
 * Runs body in a process C whose process group is orphaned: M (own group,
 * child of S) forks C (own group, child of M); when M has ended, the group
 * of C has no member whose parent is in another group of the session.
 */
static int orph_run(int slave, bodyfn body, struct rep *r)
{
	pid_t m;
	int st;

	m = fork();
	if (m == -1) {
		rep_set(r, RS_SETUP, "fork failed");
		return 0;
	}
	if (m == 0) {
		pid_t c;

		alarm(25);
		(void)setpgid(0, 0);
		c = fork();
		if (c == 0) {
			pid_t mp = getppid();
			int i;

			alarm(20);
			if (setpgid(0, 0) != 0) {
				rep_set(r, RS_SETUP, "setpgid failed: %s", pc_errname(errno));
				rep_send(r);
				_exit(0);
			}
			for (i = 0; i < 500 && getppid() == mp; i++) {
				struct timespec ts;

				ts.tv_sec = 0;
				ts.tv_nsec = 10000000L;
				nanosleep(&ts, NULL);
			}
			if (getppid() == mp) {
				rep_set(r, RS_SETUP, "the parent did not end");
				rep_send(r);
				_exit(0);
			}
			if (getsid(getppid()) == getsid(0)) {
				rep_set(r, RS_SETUP, "the orphan was adopted inside the session");
				rep_send(r);
				_exit(0);
			}
			/* The body runs in D, in this orphaned group, so that a stop can be seen and undone. */
			{
				pid_t d = fork();
				int dst;

				if (d == 0) {
					alarm(15);
					body(slave, r);
					rep_send(r);
					_exit(0);
				}
				if (d == -1) {
					rep_set(r, RS_SETUP, "fork failed");
				} else if (waitpid(d, &dst, WUNTRACED) != d) {
					rep_set(r, RS_SETUP, "waitpid failed");
				} else if (WIFSTOPPED(dst)) {
					rep_set(r, RS_STOPPED, "the caller was stopped by signal %d", WSTOPSIG(dst));
					r->v[0] = WSTOPSIG(dst);
					kill(d, SIGKILL);
					waitpid(d, &dst, 0);
				} else if (!WIFEXITED(dst) || WEXITSTATUS(dst) != 0) {
					rep_set(r, RS_SETUP, "the caller ended abnormally (status %#x)", (unsigned)dst);
				} else {
					_exit(0);
				}
				rep_send(r);
				_exit(0);
			}
		}
		_exit(c == -1 ? 1 : 0);
	}
	if (waitpid(m, &st, 0) == -1 || !WIFEXITED(st) || WEXITSTATUS(st) != 0) {
		rep_set(r, RS_SETUP, "could not start the orphaned group");
		return 0;
	}
	return 1;
}

static void leader(int mode, bodyfn body)
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
	} else if (mode != M_NOCTTY && !acquired()) {
		rep_set(&r, RS_NOCTTY, "opening the subsidiary side did not make it the controlling terminal (acquisition is implementation-defined)");
	} else if (mode == M_BG) {
		sent = bg_run(slave, body, &r);
	} else if (mode == M_ORPH) {
		sent = orph_run(slave, body, &r);
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
static struct rep run_session(int mode, bodyfn body, pid_t *spid)
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
		leader(mode, body);
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
	pc_unresolved("%s", r->msg);
}

/* For tests that expect the caller to be stopped; returns only when it was, by sig. */
static void settle_stopped(const struct rep *r, int sig)
{
	if (r->status == RS_STOPPED) {
		if (r->v[0] != sig)
			pc_fail("the caller was stopped by signal %d, not %d", r->v[0], sig);
		return;
	}
	if (r->status == RS_OK)
		pc_fail("the call returned %d (errno %s) and the caller was not stopped", r->v[0], pc_errname(r->v[1]));
	pc_unresolved("%s", r->msg);
}

/* Session of the given mode with one body: the report, after the harness problems are settled. */
static struct rep session(int mode, bodyfn body)
{
	struct rep r;
	pid_t s;

	r = run_session(mode, body, &s);
	end_session(s);
	return r;
}

/* 0 default, 1 SIGTTOU blocked, 2 SIGTTOU ignored: applied by the bodies of background tests. */
static int variant;

static void quiet_ttou(void)
{
	if (variant == 1) {
		sigset_t ss;

		sigemptyset(&ss);
		sigaddset(&ss, SIGTTOU);
		sigprocmask(SIG_BLOCK, &ss, NULL);
	} else if (variant == 2) {
		signal(SIGTTOU, SIG_IGN);
	}
}

static void body(int slave, struct rep *r)
{
	errno = 0;
	r->v[0] = (int)tcgetsid(slave);
	r->v[1] = errno;
	r->v[2] = (int)getpgrp();
}

int main(void)
{
	struct rep rep;

	pc_begin("XSH.tcgetsid.001");
	(void)open_manager();
	pc_test();
	rep = session(M_FG, body);
	settle(&rep);
	pc_check(rep.v[0] == rep.v[2], "tcgetsid() returned %d (errno %s), expected the leader's group %d", rep.v[0], pc_errname(rep.v[1]), rep.v[2]);
	pc_pass();
}
