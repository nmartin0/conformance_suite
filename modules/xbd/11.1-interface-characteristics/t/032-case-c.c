/*
 * MIN=0, TIME>0: the read timer starts with the read and ends at the first byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.1-interface-characteristics.032
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


int main(void)
{
	char b[16];
	long t0, el;
	int m, s;
	pid_t w;
	ssize_t r;

	pc_begin("XBD.11.1-interface-characteristics.032");
	s = open_pty(&m);
	cfg(s, 0, 5, 0);
	pc_test();
	t0 = now_ms();
	r = rd(s, b, sizeof b, 5);
	el = now_ms() - t0;
	pc_check(r == 0, "the read with no data returned %ld (errno %s), expected 0", (long)r, pc_errname(errno));
	pc_check(el >= 400 && el < 3000, "the read timer of 0.5 s ended after %ld ms", el);
	cfg(s, 0, 20, 0);
	w = fork();
	NEED(w != -1, "fork failed: %s", pc_errname(errno));
	if (w == 0) {
		alarm(10);
		msleep(200);
		(void)!write(m, "x", 1);
		_exit(0);
	}
	t0 = now_ms();
	r = rd(s, b, sizeof b, 5);
	el = now_ms() - t0;
	waitpid(w, NULL, 0);
	pc_check(r >= 0, "the read returned %ld (errno %s)", (long)r, pc_errname(errno));
	want("the byte that arrived during the read", b, (int)r, "x", 1);
	pc_check(el < 1500, "the byte arrived at 0.2 s but the read ended after %ld ms (TIME is 2 s)", el);
	pc_pass();
}
