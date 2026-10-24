/*
 * kill() with pid -1 sends to every process it may signal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.005
 * MUTANT    kill.all_as_self
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Installs fn for sig (FAIL or UNRESOLVED by phase). */
static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* A set holding the listed signals (end the list with 0). */
static sigset_t set_of(int first, ...)
{
	sigset_t s;
	va_list ap;
	int sig;

	sigemptyset(&s);
	va_start(ap, first);
	for (sig = first; sig != 0; sig = va_arg(ap, int))
		sigaddset(&s, sig);
	va_end(ap);
	return s;
}

/*
 * Children that report each SIGURG or SIGUSR1 they get as one byte on a
 * pipe. They live until the test's lifeline pipe closes (the test holds
 * its only write end), so none outlives the test, even in another group.
 * In a child of the test (kid_fail_exit set), setup failures exit 98
 * instead of reporting a result.
 */
static int kid_w = -1, life[2] = { -1, -1 }, kid_fail_exit;

static void on_sig(int sig)
{
	char c = sig == SIGURG ? 'u' : '1';
	(void)write(kid_w, &c, 1);
}

static void kid_fail(const char *what)
{
	if (kid_fail_exit)
		_exit(98);
	pc_unresolved("%s", what);
}

/* In a kid: install handlers, say ready, wait for the lifeline to close. */
_Noreturn static void kid_body(int p[2], int ready_w)
{
	struct sigaction sa;
	char c;

	kid_w = p[1];
	(void)close(p[0]);
	(void)close(life[1]);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGURG, &sa, NULL) != 0 || sigaction(SIGUSR1, &sa, NULL) != 0)
		_exit(99);
	(void)write(ready_w, "r", 1);
	(void)close(ready_w);
	while (read(life[0], &c, 1) == -1 && errno == EINTR)
		;
	_exit(0);
}

/* group: 0 stays in our group, 1 leads a group of its own, -g joins group g. */
static pid_t kid(int p[2], pid_t group)
{
	int ready[2];
	pid_t c;
	char ch;

	if (life[0] == -1 && pipe(life) != 0)
		kid_fail("pipe failed");
	if (pipe(ready) != 0)
		kid_fail("pipe failed");
	c = fork();
	if (c == -1)
		kid_fail("fork failed");
	if (c == 0) {
		(void)close(ready[0]);
		if (group == 1 && setpgid(0, 0) != 0)
			_exit(99);
		if (group < 0 && setpgid(0, -group) != 0)
			_exit(99);
		kid_body(p, ready[1]);
	}
	if (group == 1)
		(void)setpgid(c, c);
	(void)close(ready[1]);
	if (read(ready[0], &ch, 1) != 1) {
		(void)kill(c, SIGKILL);
		kid_fail("a child did not start");
	}
	(void)close(ready[0]);
	return c;
}

/* Counts bytes on fd, waiting up to 5 s for want of them, then 200 ms more for extras. */
static int got_until(int fd, int want)
{
	struct timespec ts = { 0, 10000000 };
	int n = 0, fl, extra = 0;
	long waited = 0;
	char c;

	fl = fcntl(fd, F_GETFL);
	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	while (waited < 5000 && extra < 200) {
		while (read(fd, &c, 1) == 1)
			n++;
		if (n >= want)
			extra += 10;
		(void)nanosleep(&ts, NULL);
		waited += 10;
	}
	while (read(fd, &c, 1) == 1)
		n++;
	(void)fcntl(fd, F_SETFL, fl);
	return n;
}

static int got(int fd, long ms)	/* for checks that nothing arrives */
{
	struct timespec ts = { 0, 10000000 };
	int n = 0, fl;
	long waited;
	char c;

	fl = fcntl(fd, F_GETFL);
	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	for (waited = 0; waited < ms; waited += 10) {
		while (read(fd, &c, 1) == 1)
			n++;
		(void)nanosleep(&ts, NULL);
	}
	while (read(fd, &c, 1) == 1)
		n++;
	(void)fcntl(fd, F_SETFL, fl);
	return n;
}

static void end(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
}

/*
 * In a child of a privileged test: become user and group u, and prove
 * that no privilege is left before signalling anything (both IDs are u,
 * and the privileged parent cannot be signalled). Exits 99 otherwise.
 */
static void become(uid_t u)
{
	if (setgid((gid_t)u) != 0 || setuid(u) != 0)
		_exit(99);
	if (getuid() != u || geteuid() != u || getgid() != (gid_t)u || getegid() != (gid_t)u)
		_exit(99);
	/* The proof is setup: planted faults (CI) stay out of it. */
	(void)setenv("PC_PHASE", "setup", 1);
	if (getppid() != 1 && (kill(getppid(), 0) != -1 || errno != EPERM))
		_exit(99);
	(void)setenv("PC_PHASE", "test", 1);
}

static void become_nobody(void)
{
	become(65534);
}

/* The otherwise unused user ID from the profile (have kill_uid U). */
static uid_t kill_uid(void)
{
	const char *v = getenv("PC_HAVE_KILL_UID");
	char *end;
	unsigned long u;

	if (v == NULL || *v == '\0')
		pc_unresolved("the profile names no unused user ID (have kill_uid U)");
	u = strtoul(v, &end, 10);
	if (*end != '\0' || u == 0)
		pc_unresolved("bad kill_uid '%s' in the profile", v);
	return (uid_t)u;
}

static volatile sig_atomic_t self_got;

static void on_self(int sig)
{
	(void)sig;
	self_got++;
}

/*
 * As user u: start a target of the same user, kill(-1, SIGURG), and
 * report "<k|e><count><s|->" (kill ok, signals the target got, the
 * sender got its own) to the parent.
 */
static void sender(uid_t u, int res_w)
{
	int p[2], r, n;
	char out[3];
	pid_t other;
	struct sigaction sa;

	become(u);
	kid_fail_exit = 1;
	if (pipe(p) != 0)
		_exit(98);
	other = kid(p, 0);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_self;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGURG, &sa, NULL) != 0)
		_exit(98);
	r = kill(-1, SIGURG);
	n = got_until(p[0], 1);
	end(other);
	out[0] = r == 0 ? 'k' : 'e';
	out[1] = (char)('0' + (n > 9 ? 9 : n));
	out[2] = self_got ? 's' : '-';
	(void)write(res_w, out, 3);
	_exit(0);
}

/* Runs sender() in a child; out gets its report. */
static void run_sender(char out[3])
{
	int res[2], st;
	uid_t u = kill_uid();
	pid_t s;
	ssize_t k;

	if (pipe(res) != 0)
		pc_unresolved("pipe failed");
	s = fork();
	if (s == -1)
		pc_unresolved("fork failed");
	if (s == 0) {
		(void)close(res[0]);
		sender(u, res[1]);
	}
	(void)close(res[1]);
	k = read(res[0], out, 3);
	if (waitpid(s, &st, 0) != s)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("cannot become user %lu without privileges left", (unsigned long)u);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the sender could not set up");
	pc_check(k == 3, "the sender did not report (status %#x)", st);
}

int main(void)
{
	char out[3];

	pc_begin("XSH.kill.005");
	pc_test();
	run_sender(out);
	pc_check(out[0] == 'k', "kill(-1, SIGURG) failed");
	pc_check(out[1] == '1', "the other process of the same user got %c signals, expected 1", out[1]);
	pc_pass();
}
