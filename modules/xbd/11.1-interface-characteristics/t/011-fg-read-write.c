/*
 * The foreground group may read and write its controlling terminal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.1-interface-characteristics.011
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

static void on_alrm(int sig)
{
	(void)sig;
}

/* SIGALRM interrupts system calls (no SA_RESTART). */
static void catch_alarm(void)
{
	static int done;
	struct sigaction sa;

	if (done)
		return;
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alrm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed: %s", pc_errname(errno));
	done = 1;
}

/* Writes all of n bytes to the manager (guarded). */
static void put_in(int m, const void *b, size_t n)
{
	const char *p = b;

	catch_alarm();
	while (n > 0) {
		ssize_t w;

		alarm(10);
		w = write(m, p, n);
		alarm(0);
		NEED(w > 0, "write to the manager failed: %s", pc_errname(errno));
		p += w;
		n -= (size_t)w;
	}
}

/* Printable form of n bytes; two buffers so one message can show two strings. */
static const char *hx(const char *b, int n, int which)
{
	static char o[2][400];
	int i, k = 0;

	o[which][0] = '\0';
	for (i = 0; i < n && k < 380; i++)
		k += snprintf(o[which] + k, sizeof o[which] - (size_t)k, "%02x ", (unsigned char)b[i]);
	return o[which];
}

static void want(const char *what, const char *got, int n, const char *exp, int en)
{
	pc_check(n == en && memcmp(got, exp, (size_t)en) == 0, "%s: got [%s], expected [%s]", what,
	    hx(got, n, 0), hx(exp, en, 1));
}

#define PUT(m, lit) put_in((m), (lit), sizeof (lit) - 1)

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

static volatile sig_atomic_t g_cnt;

static void on_cnt(int sig)
{
	(void)sig;
	g_cnt++;
}


static void sb(int slave)
{
	struct rep r;
	struct sigaction sa;
	char b[8];
	ssize_t n, w;

	memset(&r, 0, sizeof r);
	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = on_cnt;
	sigaction(SIGTTIN, &sa, NULL);
	sigaction(SIGTTOU, &sa, NULL);
	if (term_cfg(slave, 3, 0, TOSTOP) != 0) {
		rep_set(&r, RS_SETUP, "cannot set TOSTOP on the terminal");
		rep_send(&r);
		return;
	}
	rep_ok();
	n = read(slave, b, 3);
	w = write(slave, "xyz", 3);
	r.v[0] = (int)n;
	r.v[1] = (int)w;
	r.v[2] = (int)g_cnt;
	r.v[3] = n == 3 && memcmp(b, "abc", 3) == 0;
	rep_send(&r);
}

int main(void)
{
	struct rep r;
	char b[16];
	int m, nb;

	pc_begin("XBD.11.1-interface-characteristics.011");
	m = open_manager();
	start_session(SS_CTTY, sb);
	pc_test();
	go();
	rep_get(&r, 30000);
	settle(&r);
	PUT(m, "abc");
	rep_get(&r, 30000);
	end_session();
	settle(&r);
	pc_check(r.v[0] == 3 && r.v[3] == 1, "the foreground read returned %d bytes (data %s)", r.v[0], r.v[3] ? "right" : "wrong");
	pc_check(r.v[1] == 3, "the foreground write returned %d", r.v[1]);
	pc_check(r.v[2] == 0, "a job-control signal was sent to the foreground group");
	nb = collect(m, b, sizeof b, 1500);
	want("what the foreground process wrote", b, nb, "xyz", 3);
	pc_pass();
}
