/*
 * TCSADRAIN waits for output.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcsetattr.004
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

static void on_usr1(int sig)
{
	(void)sig;
}

/*
 * A child that suspends output on s, writes one byte (which is therefore
 * not transmitted) and then makes call(s); it ends with status 0 if the
 * call returned 0, else 100 plus the errno value (90 or 91 for a failed
 * set-up step). With handler set, SIGUSR1 is caught without SA_RESTART.
 */
static pid_t spawn_held(int s, int (*call)(int), int handler)
{
	pid_t c;

	fflush(NULL);
	signal(SIGUSR1, SIG_IGN);
	c = fork();
	if (c != 0)
		return c;
	alarm(20);
	if (handler) {
		struct sigaction sa;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_usr1;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGUSR1, &sa, NULL);
	}
	if (tcflow(s, TCOOFF) != 0)
		_exit(90);
	if (write(s, "x", 1) != 1)
		_exit(91);
	{
		int r = call(s);

		_exit(r == 0 ? 0 : 100 + (errno & 127));
	}
}

/* 1 (and its status) if c has ended within ms, 0 if it is still running. */
static int reap_within(pid_t c, int *st, int ms)
{
	struct timespec t0, t1;

	clock_gettime(CLOCK_MONOTONIC, &t0);
	for (;;) {
		if (waitpid(c, st, WNOHANG) == c)
			return 1;
		clock_gettime(CLOCK_MONOTONIC, &t1);
		if ((t1.tv_sec - t0.tv_sec) * 1000L + (t1.tv_nsec - t0.tv_nsec) / 1000000L >= ms)
			return 0;
		msleep(10);
	}
}

static void kill_reap(pid_t c)
{
	int st;

	kill(c, SIGKILL);
	waitpid(c, &st, 0);
}

static struct termios g_t;

static int call_set(int s)
{
	return tcsetattr(s, TCSADRAIN, &g_t);
}

int main(void)
{
	struct termios t;
	char junk[8];
	pid_t c;
	int m, s, st, orig, ended, held, unchanged;

	pc_begin("XSH.tcsetattr.004");
	s = open_pty(&m);
	NEED(tcgetattr(s, &g_t) == 0, "tcgetattr failed: %s", pc_errname(errno));
	orig = (g_t.c_lflag & ECHO) != 0;
	g_t.c_lflag ^= ECHO;
	pc_test();
	c = spawn_held(s, call_set, 0);
	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	msleep(300);
	held = !wait_in(m, 0);
	ended = reap_within(c, &st, 0);
	if (ended && WIFEXITED(st) && (WEXITSTATUS(st) == 90 || WEXITSTATUS(st) == 91))
		pc_unresolved("the child could not hold the output");
	if (!held) {
		if (!ended)
			kill_reap(c);
		pc_unresolved("the suspended output was transmitted");
	}
	if (ended)
		pc_fail("tcsetattr(TCSADRAIN) returned (status %#x) while output was suspended", (unsigned)st);
	if (tcgetattr(s, &t) != 0) {
		kill_reap(c);
		pc_unresolved("tcgetattr failed: %s", pc_errname(errno));
	}
	unchanged = ((t.c_lflag & ECHO) != 0) == orig;
	if (tcflow(s, TCOON) != 0) {
		kill_reap(c);
		pc_unresolved("tcflow(TCOON) failed: %s", pc_errname(errno));
	}
	/* The manager takes the byte now: where output counts as transmitted only once the manager has read it, the call cannot end before. */
	(void)collect(m, junk, sizeof junk, 2000);
	ended = reap_within(c, &st, 10000);
	if (!ended) {
		kill_reap(c);
		pc_fail("tcsetattr(TCSADRAIN) still blocked 10 s after the output was restarted and read");
	}
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "tcsetattr(TCSADRAIN) ended with status %#x", (unsigned)st);
	pc_check(unchanged, "the attributes were changed before the output was transmitted");
	NEED(tcgetattr(s, &t) == 0, "tcgetattr failed: %s", pc_errname(errno));
	pc_check(((t.c_lflag & ECHO) != 0) != orig, "the change was not made after the output was transmitted");
	pc_pass();
}
