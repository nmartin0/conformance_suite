/*
 * killpg() sends the signal to every process in the group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.killpg.001
 * MUTANT    killpg.leader_only
 * MUTANT    killpg.drops
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* The status of child c once it ends; FAIL (child killed) after 10 s. */
static int end_of(pid_t c)
{
	int st = 0, waited;
	pid_t r;

	for (waited = 0; waited <= 10000; waited += 10) {
		r = waitpid(c, &st, WNOHANG);
		if (r == c)
			return st;
		if (r == -1 && errno != EINTR)
			pc_check(0, "waitpid failed: %s", pc_errname(errno));
		sleep_ms(10);
	}
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
	pc_check(0, "the child did not end within 10 s");
	return 0;
}

/* Every signal the header names, and how many. */
static const int table[] = {
	SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCONT, SIGFPE, SIGHUP, SIGILL, SIGINT,
	SIGKILL, SIGPIPE, SIGQUIT, SIGSEGV, SIGSTOP, SIGTERM, SIGTSTP, SIGTTIN,
	SIGTTOU, SIGUSR1, SIGUSR2, SIGURG, SIGWINCH,
#ifdef SIGSYS
	SIGSYS,
#endif
#ifdef SIGTRAP
	SIGTRAP,
#endif
#ifdef SIGVTALRM
	SIGVTALRM,
#endif
#ifdef SIGXCPU
	SIGXCPU,
#endif
#ifdef SIGXFSZ
	SIGXFSZ,
#endif
};
static const char *const table_name[] = {
	"ABRT", "ALRM", "BUS", "CHLD", "CONT", "FPE", "HUP", "ILL", "INT",
	"KILL", "PIPE", "QUIT", "SEGV", "STOP", "TERM", "TSTP", "TTIN",
	"TTOU", "USR1", "USR2", "URG", "WINCH",
#ifdef SIGSYS
	"SYS",
#endif
#ifdef SIGTRAP
	"TRAP",
#endif
#ifdef SIGVTALRM
	"VTALRM",
#endif
#ifdef SIGXCPU
	"XCPU",
#endif
#ifdef SIGXFSZ
	"XFSZ",
#endif
};
#define NTABLE ((int)(sizeof table / sizeof table[0]))

/*
 * A process group of two children (leader and member) that report each
 * signal they get on a pipe; the leader's pid is the group ID.
 */
static int rep[2];

static void on_sig(int sig)
{
	char c = (char)(sig == SIGUSR1 ? 'u' : '?');

	(void)write(rep[1], &c, 1);
}

static pid_t made[2];
static int nmade;

static void kill_made(void)
{
	int i;

	for (i = 0; i < nmade; i++)
		(void)kill(made[i], SIGKILL);
}

static pid_t member(pid_t pgid, int ready)
{
	pid_t c = fork();

	pc_check(c != -1, "fork failed");
	if (c == 0) {
		struct sigaction sa;
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_sig;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(SIGUSR1, &sa, NULL);
		(void)setpgid(0, pgid);
		(void)write(ready, "r", 1);
		for (;;)
			(void)pause();
	}
	(void)setpgid(c, pgid ? pgid : c);
	if (nmade == 0)
		(void)atexit(kill_made);
	if (nmade < 2)
		made[nmade++] = c;
	return c;
}

static int count_within(int fd, int want, long ms)
{
	int got = 0, waited;
	char c;
	int fl = fcntl(fd, F_GETFL);

	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	for (waited = 0; waited <= ms && got < want; waited += 10) {
		while (read(fd, &c, 1) == 1)
			got++;
		if (got < want)
			sleep_ms(10);
	}
	return got;
}

static volatile sig_atomic_t self_got;

static void self_h(int sig)
{
	(void)sig;
	self_got = 1;
}

int main(void)
{
	int ready[2], got, r;
	pid_t a, b;
	char c;

	pc_begin("XSH.killpg.001");
	pc_check(pipe(rep) == 0 && pipe(ready) == 0, "pipe failed");
	catch_sig(SIGUSR1, self_h);
	a = member(0, ready[1]);
	b = member(a, ready[1]);
	pc_check(read(ready[0], &c, 1) == 1 && read(ready[0], &c, 1) == 1, "the children did not start");
	pc_test();
	r = killpg(a, SIGUSR1);
	got = count_within(rep[0], 2, 5000);
	(void)kill(a, SIGKILL);
	(void)kill(b, SIGKILL);
	(void)waitpid(a, NULL, 0);
	(void)waitpid(b, NULL, 0);
	pc_check(r == 0, "killpg() returned %d (%s)", r, pc_errname(errno));
	pc_check(got == 2, "%d of the 2 group members got the signal", got);
	pc_check(!self_got, "the caller, outside the group, got the signal");
	pc_pass();
}
