/*
 * The name belongs to that manager.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ptsname.003
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

/* A manager made ready (granted, unlocked); the name from ptsname() is copied to nm. */
static int ready_manager(char *nm, size_t sz)
{
	const char *p;
	int m = posix_openpt(O_RDWR | O_NOCTTY);

	if (m < 0) {
		if (errno == ENOENT || errno == ENOSYS)
			pc_notinuse("no pseudo-terminal facility: %s", pc_errname(errno));
		pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
	}
	if (grantpt(m) != 0 || unlockpt(m) != 0)
		pc_unresolved("cannot set up the pseudo-terminal: %s", pc_errname(errno));
	p = ptsname(m);
	if (p == NULL || strlen(p) >= sz)
		pc_unresolved("ptsname failed: %s", pc_errname(errno));
	strcpy(nm, p);
	return m;
}

static void raw(int s)
{
	struct termios t;

	if (tcgetattr(s, &t) != 0)
		pc_unresolved("tcgetattr failed");
	t.c_iflag &= ~(tcflag_t)(ICRNL | INLCR | IGNCR | IXON | IXOFF);
	t.c_oflag &= ~(tcflag_t)OPOST;
	t.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ISIG | IEXTEN);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(s, TCSANOW, &t) != 0)
		pc_unresolved("tcsetattr failed");
}

static int rd(int fd, char *b, int ms)
{
	struct pollfd pf;

	pf.fd = fd;
	pf.events = POLLIN;
	if (poll(&pf, 1, ms) <= 0)
		return 0;
	return (int)read(fd, b, 8);
}

int main(void)
{
	char n1[256], n2[256], b[8];
	int m1, m2, s1, s2;

	pc_begin("XSH.ptsname.003");
	m1 = ready_manager(n1, sizeof n1);
	m2 = ready_manager(n2, sizeof n2);
	s1 = open(n1, O_RDWR | O_NOCTTY);
	s2 = open(n2, O_RDWR | O_NOCTTY);
	NEED(s1 >= 0 && s2 >= 0, "cannot open the subsidiaries");
	raw(s1);
	raw(s2);
	pc_test();
	NEED(write(m1, "A", 1) == 1 && write(m2, "B", 1) == 1, "write failed");
	pc_check(rd(s1, b, 2000) == 1 && b[0] == 'A', "the first subsidiary did not get the byte written to the first manager");
	pc_check(rd(s2, b, 2000) == 1 && b[0] == 'B', "the second subsidiary did not get the byte written to the second manager");
	pc_check(rd(s1, b, 200) == 0 && rd(s2, b, 200) == 0, "a byte arrived on the wrong subsidiary");
	pc_pass();
}
