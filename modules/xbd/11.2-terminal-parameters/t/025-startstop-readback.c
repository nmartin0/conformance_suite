/*
 * The START and STOP bytes read back from the terminal are the ones in use.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.2-terminal-parameters.025
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

static void cfg(int fd, int vmin, int vtime, tcflag_t lset)
{
	NEED(term_cfg(fd, vmin, vtime, lset) == 0, "cannot put the terminal in the mode the test needs: %s", pc_errname(errno));
}

static void get_attr(int s, struct termios *t)
{
	NEED(tcgetattr(s, t) == 0, "tcgetattr failed: %s", pc_errname(errno));
}

static void put_attr(int s, const struct termios *t)
{
	NEED(tcsetattr(s, TCSANOW, t) == 0, "tcsetattr failed: %s", pc_errname(errno));
}

/* Sets and clears c_iflag bits, and checks that the terminal kept them. */
static void iflags(int s, tcflag_t set, tcflag_t clr)
{
	struct termios t;

	get_attr(s, &t);
	t.c_iflag |= set;
	t.c_iflag &= ~clr;
	put_attr(s, &t);
	get_attr(s, &t);
	pc_check((t.c_iflag & set) == set && (t.c_iflag & clr) == 0, "tcsetattr() did not keep the c_iflag change");
}

/* The value that disables a special character, or -1 when the terminal has none. */
static int vdis(int fd)
{
	long v;

	errno = 0;
	v = fpathconf(fd, _PC_VDISABLE);
	return v >= 0 && v <= 255 ? (int)v : -1;
}

/* A write on the subsidiary side. */
static void sput(int s, const char *p, int n)
{
	NEED(write(s, p, (size_t)n) == n, "write to the subsidiary side failed: %s", pc_errname(errno));
}

/* What the subsidiary side wrote, as the manager receives it, must be exactly exp. */
static void expect_out(int m, const char *what, const char *exp, int en)
{
	char b[512];
	int n = collect(m, b, sizeof b, 1500);

	want(what, b, n, exp, en);
}

static void expect_quiet(int m, const char *what)
{
	char b[64];
	int n = collect(m, b, sizeof b, 400);

	pc_check(n == 0, "%s: %d bytes arrived at the manager [%s]", what, n, hx(b, n, 0));
}


int main(void)
{
	struct termios t;
	char stop, start;
	int m, s, v;

	pc_begin("XBD.11.2-terminal-parameters.025");
	s = open_pty(&m);
	cfg(s, 0, 0, 0);
	pc_test();
	get_attr(s, &t);
	t.c_cc[VSTOP] = 0x1d;
	t.c_cc[VSTART] = 0x1f;
	put_attr(s, &t);
	get_attr(s, &t);
	stop = (char)t.c_cc[VSTOP];
	start = (char)t.c_cc[VSTART];
	v = vdis(s);
	if (v >= 0 && ((unsigned char)stop == v || (unsigned char)start == v))
		pc_notinuse("the terminal has no START or STOP character in use");
	NEED(stop != start, "START and STOP are the same byte");
	iflags(s, IXON, IXANY);
	sput(s, "y", 1);
	expect_out(m, "output before STOP", "y", 1);
	put_in(m, &stop, 1);
	msleep(150);
	sput(s, "x", 1);
	expect_quiet(m, "output after the STOP byte read back");
	put_in(m, &start, 1);
	expect_out(m, "output after the START byte read back", "x", 1);
	pc_pass();
}
