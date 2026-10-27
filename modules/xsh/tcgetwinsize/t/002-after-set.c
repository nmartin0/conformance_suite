/*
 * tcgetwinsize() returns what tcsetwinsize() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcgetwinsize.002
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

static int ws_eq(const struct winsize *a, unsigned short r, unsigned short c)
{
	return a->ws_row == r && a->ws_col == c;
}

int main(void)
{
	struct winsize w, g;
	int m, s;

	pc_begin("XSH.tcgetwinsize.002");
	s = open_pty(&m);
	NEED(tcgetwinsize(s, &w) == 0, "tcgetwinsize failed: %s", pc_errname(errno));
	pc_test();
	w.ws_row = 31;
	w.ws_col = 97;
	NEED(tcsetwinsize(s, &w) == 0, "tcsetwinsize failed: %s", pc_errname(errno));
	pc_check(tcgetwinsize(s, &g) == 0, "tcgetwinsize() failed: %s", pc_errname(errno));
	pc_check(ws_eq(&g, 31, 97), "got %ux%u, set 31x97", (unsigned)g.ws_row, (unsigned)g.ws_col);
	w.ws_row = 7;
	w.ws_col = 200;
	NEED(tcsetwinsize(s, &w) == 0, "tcsetwinsize failed: %s", pc_errname(errno));
	pc_check(tcgetwinsize(s, &g) == 0, "tcgetwinsize() failed: %s", pc_errname(errno));
	pc_check(ws_eq(&g, 7, 200), "got %ux%u, set 7x200", (unsigned)g.ws_row, (unsigned)g.ws_col);
	pc_pass();
}
