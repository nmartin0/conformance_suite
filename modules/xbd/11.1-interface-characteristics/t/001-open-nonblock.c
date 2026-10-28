/*
 * Opening a terminal with O_NONBLOCK, or with CLOCAL set, does not wait.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.1-interface-characteristics.001
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


int main(void)
{
	struct termios t;
	int m, f1, f2, clocal = 0;

	pc_begin("XBD.11.1-interface-characteristics.001");
	m = open_manager();
	catch_alarm();
	pc_test();
	alarm(10);
	f1 = open(ptsn, O_RDWR | O_NOCTTY | O_NONBLOCK);
	alarm(0);
	pc_check(f1 >= 0, "open() with O_NONBLOCK of the subsidiary side failed or did not return: %s", pc_errname(errno));
	if (tcgetattr(f1, &t) == 0) {
		t.c_cflag |= CLOCAL;
		if (tcsetattr(f1, TCSANOW, &t) == 0 && tcgetattr(f1, &t) == 0 && (t.c_cflag & CLOCAL))
			clocal = 1;
	}
	if (clocal) {
		alarm(10);
		f2 = open(ptsn, O_RDWR | O_NOCTTY);
		alarm(0);
		pc_check(f2 >= 0, "open() without O_NONBLOCK, with CLOCAL set, failed or did not return: %s", pc_errname(errno));
	} else {
		pc_note("CLOCAL is not kept on this terminal: only the O_NONBLOCK case was checked");
	}
	(void)m;
	pc_pass();
}
