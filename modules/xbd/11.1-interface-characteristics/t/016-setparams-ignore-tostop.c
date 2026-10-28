/*
 * Calls that set terminal parameters act like writes with TOSTOP set, whatever TOSTOP is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.1-interface-characteristics.016
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

static void set_chars(struct termios *t)
{
	t->c_cc[VERASE] = 0x08;
	t->c_cc[VKILL] = 0x15;
	t->c_cc[VEOF] = 0x04;
	t->c_cc[VEOL] = 0x1e;
	t->c_cc[VINTR] = 0x03;
	t->c_cc[VQUIT] = 0x1c;
	t->c_cc[VSUSP] = 0x1a;
}

/*
 * Everything off ("raw"), known special characters, VMIN and VTIME as given (left alone when
 * ICANON is requested, as they may share slots with EOF and EOL), then the c_lflag bits lset
 * on. Returns 0, or -1 on any failure.
 */
static int term_cfg(int fd, int vmin, int vtime, tcflag_t lset)
{
	struct termios t;

	if (tcgetattr(fd, &t) != 0)
		return -1;
	t.c_iflag &= ~(tcflag_t)(ICRNL | INLCR | IGNCR | IXON | IXOFF | ISTRIP | PARMRK | INPCK | BRKINT | IXANY);
	t.c_oflag &= ~(tcflag_t)OPOST;
	t.c_cflag &= ~(tcflag_t)(CSIZE | PARENB);
	t.c_cflag |= CS8 | CREAD;
	t.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN | NOFLSH | TOSTOP);
	t.c_lflag |= lset;
	set_chars(&t);
	if (!(lset & ICANON)) {
		t.c_cc[VMIN] = (cc_t)vmin;
		t.c_cc[VTIME] = (cc_t)vtime;
	}
	if (tcsetattr(fd, TCSANOW, &t) != 0)
		return -1;
	if (tcgetattr(fd, &t) != 0)
		return -1;
	if ((t.c_lflag & lset) != lset || (!(lset & ICANON) && (t.c_lflag & ICANON)))
		return -1;
	if (t.c_cc[VERASE] != 0x08 || t.c_cc[VKILL] != 0x15 || t.c_cc[VINTR] != 0x03 || t.c_cc[VQUIT] != 0x1c ||
	    t.c_cc[VSUSP] != 0x1a)
		return -1;
	if ((lset & ICANON) && (t.c_cc[VEOF] != 0x04 || t.c_cc[VEOL] != 0x1e))
		return -1;
	if (!(lset & ICANON) && (t.c_cc[VMIN] != vmin || t.c_cc[VTIME] != vtime))
		return -1;
	return 0;
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
static pid_t g_sess = -1;

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

/* Default action, and not blocked, for sig in the calling process. */
static void dfl(int sig)
{
	sigset_t ss;

	signal(sig, SIG_DFL);
	sigemptyset(&ss);
	sigaddset(&ss, sig);
	sigprocmask(SIG_UNBLOCK, &ss, NULL);
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
	g_sess = s;
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

typedef void (*bodyfn)(int slave, struct rep *r);

/* Runs body in a background child A of S (own group, not orphaned); returns 1 if A sent the report. */
static int bg_run(int slave, bodyfn body, struct rep *r)
{
	int go_[2], st;
	pid_t a;

	if (pipe(go_) != 0) {
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

		close(go_[1]);
		alarm(20);
		if (setpgid(0, 0) != 0) {
			rep_set(r, RS_SETUP, "setpgid failed: %s", pc_errname(errno));
			rep_send(r);
			_exit(0);
		}
		if (read(go_[0], &c, 1) != 1)
			_exit(1);
		body(slave, r);
		rep_send(r);
		_exit(0);
	}
	close(go_[0]);
	(void)setpgid(a, a);
	if (write(go_[1], "g", 1) != 1) {
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
 * Runs body in a process whose group is orphaned: M (own group, child of S) forks C (own group,
 * child of M); when M has ended, the group of C has no member whose parent is in another group of
 * the session. The body runs in D, a child of C, so that a stop can be seen and undone.
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
			pid_t d;
			int i, dst;

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
			d = fork();
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
		_exit(c == -1 ? 1 : 0);
	}
	if (waitpid(m, &st, 0) == -1 || !WIFEXITED(st) || WEXITSTATUS(st) != 0) {
		rep_set(r, RS_SETUP, "could not start the orphaned group");
		return 0;
	}
	return 1;
}

static bodyfn g_body;
static int (*g_prep)(int slave);

static void sb_bg(int slave)
{
	struct rep r;
	int sent;

	memset(&r, 0, sizeof r);
	if (g_prep && g_prep(slave) != 0) {
		rep_set(&r, RS_SETUP, "cannot set the terminal mode the test needs");
		rep_send(&r);
		return;
	}
	sent = bg_run(slave, g_body, &r);
	if (!sent)
		rep_send(&r);
}

static void sb_orph(int slave)
{
	struct rep r;
	int sent;

	memset(&r, 0, sizeof r);
	if (g_prep && g_prep(slave) != 0) {
		rep_set(&r, RS_SETUP, "cannot set the terminal mode the test needs");
		rep_send(&r);
		return;
	}
	sent = orph_run(slave, g_body, &r);
	if (!sent)
		rep_send(&r);
}

/* One session: prep runs in S, body in a background (orphan = 0) or orphaned (orphan = 1) caller. */
static struct rep run_case(int orphan, bodyfn body, int (*prep)(int))
{
	struct rep r;

	g_body = body;
	g_prep = prep;
	start_session(SS_CTTY, orphan ? sb_orph : sb_bg);
	go();
	rep_get(&r, 30000);
	end_session();
	return r;
}

/* The expected outcome: stopped by sig. */
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

static volatile sig_atomic_t g_cnt;

static void on_cnt(int sig)
{
	(void)sig;
	g_cnt++;
}

static void on_noop(int sig)
{
	(void)sig;
}

/* A counting handler for sig (0 for none), and SIGALRM interrupting system calls; arms an alarm. */
static void count_and_alarm(int sig, unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	if (sig) {
		sa.sa_handler = on_cnt;
		sigaction(sig, &sa, NULL);
	}
	sa.sa_handler = on_noop;
	sigaction(SIGALRM, &sa, NULL);
	alarm(secs);
}


static int which;

static int prep(int slave)
{
	return term_cfg(slave, 1, 0, 0);
}

static void body(int slave, struct rep *r)
{
	struct termios t;
	int rc;

	dfl(SIGTTOU);
	count_and_alarm(0, 3);
	if (tcgetattr(slave, &t) != 0) {
		rep_set(r, RS_SETUP, "tcgetattr failed: %s", pc_errname(errno));
		return;
	}
	errno = 0;
	switch (which) {
	case 0:
		rc = tcsetattr(slave, TCSANOW, &t);
		break;
	case 1:
		rc = tcdrain(slave);
		break;
	case 2:
		rc = tcflow(slave, TCOON);
		break;
	case 3:
		rc = tcflush(slave, TCIFLUSH);
		break;
	default:
		rc = tcsendbreak(slave, 0);
		break;
	}
	r->v[0] = rc;
	r->v[1] = errno;
}

int main(void)
{
	static const char *const names[5] = {"tcsetattr", "tcdrain", "tcflow", "tcflush", "tcsendbreak"};
	struct rep r;
	int m;

	pc_begin("XBD.11.1-interface-characteristics.016");
	m = open_manager();
	(void)m;
	pc_test();
	for (which = 0; which < 5; which++) {
		pc_note("function: %s", names[which]);
		r = run_case(0, body, prep);
		settle_stopped(&r, SIGTTOU);
	}
	pc_pass();
}
