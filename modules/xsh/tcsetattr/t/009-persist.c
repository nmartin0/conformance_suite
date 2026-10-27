/*
 * Attributes outlive one descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcsetattr.009
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

int main(void)
{
	struct termios t, g;
	int m, s1, s2;

	pc_begin("XSH.tcsetattr.009");
	s1 = open_pty(&m);
	s2 = open(ptsn, O_RDWR | O_NOCTTY);
	NEED(s2 >= 0, "second open failed: %s", pc_errname(errno));
	NEED(tcgetattr(s1, &t) == 0, "tcgetattr failed: %s", pc_errname(errno));
	t.c_lflag &= ~(tcflag_t)(ECHO | ICANON);
	t.c_cc[VMIN] = 4;
	pc_test();
	pc_check(tcsetattr(s1, TCSANOW, &t) == 0, "tcsetattr() failed: %s", pc_errname(errno));
	NEED(close(s1) == 0, "close failed");
	NEED(tcgetattr(s2, &g) == 0, "tcgetattr failed: %s", pc_errname(errno));
	pc_check((g.c_lflag & (ECHO | ICANON)) == 0 && g.c_cc[VMIN] == 4, "the change did not survive the close of the other descriptor");
	pc_pass();
}
