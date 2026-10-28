/*
 * Without ICANON the disabling value has no special meaning for MIN and TIME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.2-terminal-parameters.024
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

static long now_ms(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
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

/* read() guarded by an alarm of secs seconds: -1 with EINTR when it gives up. */
static ssize_t rd(int fd, void *b, size_t n, unsigned secs)
{
	ssize_t r;
	int e;

	catch_alarm();
	alarm(secs);
	r = read(fd, b, n);
	e = errno;
	alarm(0);
	errno = e;
	return r;
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

#define PUT(m, lit) put_in((m), (lit), sizeof (lit) - 1)

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

/* The value that disables a special character, or -1 when the terminal has none. */
static int vdis(int fd)
{
	long v;

	errno = 0;
	v = fpathconf(fd, _PC_VDISABLE);
	return v >= 0 && v <= 255 ? (int)v : -1;
}


int main(void)
{
	char b[16];
	struct termios t;
	long t0, el, ms;
	int m, s, v;
	ssize_t r;

	pc_begin("XBD.11.2-terminal-parameters.024");
	s = open_pty(&m);
	cfg(s, 1, 0, 0);
	v = vdis(s);
	if (v < 0)
		pc_notinuse("the terminal has no disabling value for special characters");
	ms = 100L * v;
	{
		long mi = fpathconf(s, _PC_MAX_INPUT);

		NEED(mi < 0 || v <= mi, "the disabling value %d is above MAX_INPUT (%ld): MIN would be undefined", v, mi);
	}
	NEED(v < 20 || v > 35, "the disabling value %d is too close to the alarm for the TIME check", v);
	pc_test();
	get_attr(s, &t);
	t.c_cc[VMIN] = (cc_t)v;
	t.c_cc[VTIME] = 0;
	put_attr(s, &t);
	PUT(m, "a");
	msleep(100);
	r = rd(s, b, sizeof b, 1);
	if (v <= 1)
		pc_check(r == 1 && b[0] == 'a', "MIN %d: the read of one queued byte returned %ld (errno %s)", v, (long)r, pc_errname(errno));
	else
		pc_check(r == -1 && errno == EINTR, "MIN %d: the read of one queued byte returned %ld (errno %s) instead of waiting", v, (long)r,
		    pc_errname(errno));
	NEED(tcflush(s, TCIFLUSH) == 0, "tcflush failed: %s", pc_errname(errno));
	get_attr(s, &t);
	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = (cc_t)v;
	put_attr(s, &t);
	t0 = now_ms();
	r = rd(s, b, sizeof b, 3);
	el = now_ms() - t0;
	if (v == 0)
		pc_check(r == 0 && el < 1000, "TIME 0: the empty read returned %ld after %ld ms", (long)r, el);
	else if (v < 20)
		pc_check(r == 0 && el >= ms - 100 && el < ms + 2000, "TIME %d: the empty read returned %ld after %ld ms", v, (long)r, el);
	else
		pc_check(r == -1 && errno == EINTR, "TIME %d: the empty read returned %ld (errno %s) instead of waiting", v, (long)r, pc_errname(errno));
	pc_pass();
}
