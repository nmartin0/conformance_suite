/*
 * A child spawned with POSIX_SPAWN_SETSCHEDULER and a spawn-schedpolicy set through...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getschedpolicy.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

extern char **environ;

static char *self;		/* this program, the child image */
static int rfd = -1;		/* in the image: where its report goes */
static volatile sig_atomic_t timed_out;

struct kid {
	pid_t pid;
	int fd;			/* read end of the report pipe */
	char buf[512];
	size_t len;
};

/* In the image: adds to the report. */
static void say(const char *fmt, ...)
{
	char b[400];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vsnprintf(b, sizeof b, fmt, ap);
	va_end(ap);
	if (n > 0)
		(void)write(rfd, b, (size_t)n < sizeof b ? (size_t)n : sizeof b - 1);
}

/*
 * The image. Modes: ran; fds (each argument a descriptor: open or closed,
 * FD_CLOEXEC, access mode); read (reads from the descriptor given); write;
 * cwd (device and serial number of "."); sig (each argument a signal:
 * blocked, and the action i, d or h); grp (process group and process ID);
 * sched (policy and priority); hold (reports, waits for end of file on the
 * descriptor given); after (waits for one byte on the descriptor given,
 * then runs the mode that follows).
 */
static void image(const char *mode, int n, char **a)
{
	int i;

	if (strcmp(mode, "ran") == 0) {
		say("[ran]");
	} else if (strcmp(mode, "fds") == 0) {
		for (i = 0; i < n; i++) {
			int fd = atoi(a[i]), fl = fcntl(fd, F_GETFL), fdfl = fcntl(fd, F_GETFD);

			if (fl == -1)
				say("[%d:c]", fd);
			else
				say("[%d:o:%d:%d]", fd, fdfl != -1 && (fdfl & FD_CLOEXEC) != 0, fl & O_ACCMODE);
		}
	} else if (strcmp(mode, "read") == 0 && n >= 1) {
		char b[64];
		ssize_t r = read(atoi(a[0]), b, sizeof b - 1);

		if (r < 0) {
			say("[R!%d]", errno);
		} else {
			b[r] = '\0';
			say("[R:%s]", b);
		}
	} else if (strcmp(mode, "write") == 0 && n >= 1) {
		say("[W:%d]", (int)write(atoi(a[0]), "x", 1));
	} else if (strcmp(mode, "cwd") == 0) {
		struct stat st;

		if (stat(".", &st) != 0)
			say("[D!]");
		else
			say("[D:%ju:%ju]", (uintmax_t)st.st_dev, (uintmax_t)st.st_ino);
	} else if (strcmp(mode, "sig") == 0) {
		sigset_t cur;

		(void)sigprocmask(SIG_BLOCK, NULL, &cur);
		for (i = 0; i < n; i++) {
			int sig = atoi(a[i]);
			struct sigaction sa;
			char c = '?';

			if (sigaction(sig, NULL, &sa) == 0)
				c = sa.sa_handler == SIG_IGN ? 'i' : sa.sa_handler == SIG_DFL ? 'd' : 'h';
			say("[%d:%d:%c]", sig, sigismember(&cur, sig) == 1, c);
		}
	} else if (strcmp(mode, "grp") == 0) {
		say("[G:%ld:%ld]", (long)getpgrp(), (long)getpid());
	} else if (strcmp(mode, "sched") == 0) {
		struct sched_param sp;

		memset(&sp, 0, sizeof sp);
		(void)sched_getparam(0, &sp);
		say("[S:%d:%d]", sched_getscheduler(0), sp.sched_priority);
	} else if (strcmp(mode, "hold") == 0 && n >= 1) {
		char b[16];

		say("[up]");
		while (read(atoi(a[0]), b, sizeof b) > 0)
			;
	} else if (strcmp(mode, "after") == 0 && n >= 2) {
		char b[1];

		if (read(atoi(a[0]), b, 1) == 1)
			image(a[1], n - 2, a + 2);
	} else {
		say("[?mode]");
	}
}

static void on_alarm(int sig)
{
	(void)sig;
	timed_out = 1;
}

static void kill_wait(pid_t p)
{
	(void)kill(p, SIGKILL);
	(void)waitpid(p, NULL, 0);
}

/* Starts the image in mode m with the arguments given (NULL ended). */
static int start(const posix_spawn_file_actions_t *fa, const posix_spawnattr_t *at,
    const char *m, const char *const *args, struct kid *k)
{
	int p[2], r, h, n = 0;
	char num[16];
	char *av[24];

	if (pipe(p) != 0)
		pc_unresolved("pipe failed");
	/* The pipe goes to high numbers, so that low numbers stay as the test left them. */
	if ((h = fcntl(p[0], F_DUPFD, (sysconf(_SC_OPEN_MAX) > 0 && sysconf(_SC_OPEN_MAX) < 400 ? (int)(sysconf(_SC_OPEN_MAX) / 2) : 200))) != -1) {
		(void)close(p[0]);
		p[0] = h;
	}
	if ((h = fcntl(p[1], F_DUPFD, (sysconf(_SC_OPEN_MAX) > 0 && sysconf(_SC_OPEN_MAX) < 400 ? (int)(sysconf(_SC_OPEN_MAX) / 2) : 200))) != -1) {
		(void)close(p[1]);
		p[1] = h;
	}
	(void)fcntl(p[0], F_SETFD, FD_CLOEXEC);
	(void)snprintf(num, sizeof num, "%d", p[1]);
	av[n++] = self;
	av[n++] = "img";
	av[n++] = (char *)m;
	av[n++] = num;
	while (args != NULL && *args != NULL && n < 22)
		av[n++] = (char *)*args++;
	av[n] = NULL;
	k->pid = -1;
	k->len = 0;
	k->buf[0] = '\0';
	r = posix_spawn(&k->pid, self, fa, at, av, environ);
	(void)close(p[1]);
	if (r != 0) {
		(void)close(p[0]);
		return r;
	}
	k->fd = p[0];
	return 0;
}

/* Reads the report until the token appears (or to end of file when tok is NULL). */
static void await(struct kid *k, const char *tok)
{
	ssize_t r;

	timed_out = 0;
	(void)alarm(20);
	while (k->len < sizeof k->buf - 1) {
		if (tok != NULL && strstr(k->buf, tok) != NULL)
			break;
		r = read(k->fd, k->buf + k->len, sizeof k->buf - 1 - k->len);
		if (r > 0) {
			k->len += (size_t)r;
			k->buf[k->len] = '\0';
		} else if (r == 0) {
			break;
		} else if (errno == EINTR && timed_out) {
			kill_wait(k->pid);
			pc_unresolved("the child did not report within 20 s");
		} else if (errno != EINTR) {
			break;
		}
	}
	(void)alarm(0);
}

/* Reads to the end of the report and reaps the child; the wait status. */
static int finish(struct kid *k)
{
	int st = 0;

	await(k, NULL);
	(void)close(k->fd);
	timed_out = 0;
	(void)alarm(20);
	while (waitpid(k->pid, &st, 0) == -1) {
		if (errno == EINTR && !timed_out)
			continue;
		if (errno == EINTR) {
			kill_wait(k->pid);
			pc_unresolved("the child did not end within 20 s");
		}
		pc_unresolved("waitpid failed");
	}
	(void)alarm(0);
	return st;
}

/* Starts and finishes: posix_spawn()'s result; with 0, the report is in buf. */
static int launch(const posix_spawn_file_actions_t *fa, const posix_spawnattr_t *at,
    const char *m, const char *const *args, char *buf, size_t sz, int *st)
{
	struct kid k;
	int r = start(fa, at, m, args, &k);

	if (r != 0)
		return r;
	*st = finish(&k);
	(void)snprintf(buf, sz, "%s", k.buf);
	return 0;
}

/* A spawn that must work and end with status 0; the report is in buf. */
static void run(const posix_spawn_file_actions_t *fa, const posix_spawnattr_t *at,
    const char *m, const char *const *args, char *buf, size_t sz)
{
	int st = 0, r = launch(fa, at, m, args, buf, sz, &st);

	pc_check(r == 0, "posix_spawn() returned %s", pc_errname(r));
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0,
	    "the child ended with status %#x (127 means that a file action or attribute failed); report \"%s\"", st, buf);
}

/* A spawn that must not run the image: an error number, or the child's status 127. */
static void must_not_run(const posix_spawn_file_actions_t *fa, const posix_spawnattr_t *at, const char *what)
{
	char buf[512];
	int st = 0, r = launch(fa, at, "ran", NULL, buf, sizeof buf, &st);

	if (r != 0)
		return;
	pc_check(strstr(buf, "[ran]") == NULL, "%s: the image ran", what);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 127,
	    "%s: posix_spawn() returned 0 and the child ended with status %#x, not 127", what, st);
}

static int has(const char *buf, const char *fmt, ...)
{
	char b[128];
	va_list ap;

	va_start(ap, fmt);
	(void)vsnprintf(b, sizeof b, fmt, ap);
	va_end(ap);
	return strstr(buf, b) != NULL;
}

/* A descriptor number that is in range and not open. */
static int free_fd(void)
{
	int f = open("/dev/null", O_RDONLY);

	if (f == -1)
		pc_unresolved("cannot open /dev/null");
	(void)close(f);
	return f;
}

/* A path made absolute with the working directory. */
static void abspath(char *out, size_t sz, const char *name)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed");
	(void)snprintf(out, sz, "%s/%s", cwd, name);
}

static void ident(const char *path, uintmax_t *dev, uintmax_t *ino)
{
	struct stat st;

	if (stat(path, &st) != 0)
		pc_unresolved("cannot stat %s", path);
	*dev = (uintmax_t)st.st_dev;
	*ino = (uintmax_t)st.st_ino;
}

static long open_max(void)
{
	long om = sysconf(_SC_OPEN_MAX);

	if (om < 1 || om > INT_MAX)
		pc_unresolved("{OPEN_MAX} is indeterminate or too large for int");
	return om;
}

static void init_common(char **argv)
{
	struct sigaction sa;
	char buf[512];
	int st = 0, r;

	if (argv[0][0] != '/')
		pc_unresolved("argv[0] is not an absolute path");
	self = argv[0];
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) != 0)
		pc_unresolved("sigaction failed");
	/* Control: a plain spawn of the image must work, or nothing below means anything. */
	r = launch(NULL, NULL, "ran", NULL, buf, sizeof buf, &st);
	if (r != 0 || strstr(buf, "[ran]") == NULL)
		pc_unresolved("a plain posix_spawn() of the test image does not work (%s)", pc_errname(r));
}

static pid_t group_of(const char *buf)
{
	long g = -1, p = -1;
	const char *s = strstr(buf, "[G:");

	if (s != NULL)
		(void)sscanf(s, "[G:%ld:%ld]", &g, &p);
	return (pid_t)g;
}

static pid_t pid_of(const char *buf)
{
	long g = -1, p = -1;
	const char *s = strstr(buf, "[G:");

	if (s != NULL)
		(void)sscanf(s, "[G:%ld:%ld]", &g, &p);
	return (pid_t)p;
}

int main(int argc, char **argv)
{
	if (argc >= 4 && strcmp(argv[1], "img") == 0) {
		rfd = atoi(argv[3]);
		image(argv[2], argc - 4, argv + 4);
		return 0;
	}
	pc_begin("XSH.posix_spawnattr_getschedpolicy.003");
	init_common(argv);

	posix_spawnattr_t at;
	struct sched_param sp;
	char buf[512];
	int cur, pol = -1, prio = -1, i, r, st = 0;
	const int pols[2] = { SCHED_FIFO, SCHED_RR };

#if defined(POSIX_SPAWN_SETSCHEDULER)
	cur = sched_getscheduler(0);
	for (i = 0; i < 2 && pol == -1; i++)
		if (pols[i] != cur && sched_get_priority_min(pols[i]) != -1) {
			pol = pols[i];
			prio = sched_get_priority_min(pols[i]);
		}
	if (pol == -1)
		pc_unresolved("no second scheduling policy to try");
	memset(&sp, 0, sizeof sp);
	sp.sched_priority = prio;
	pc_check(posix_spawnattr_init(&at) == 0, "posix_spawnattr_init failed");
	pc_check(posix_spawnattr_setschedparam(&at, &sp) == 0, "setschedparam failed");
	pc_check(posix_spawnattr_setflags(&at, POSIX_SPAWN_SETSCHEDULER) == 0, "setflags failed");
	pc_test();
	r = posix_spawnattr_setschedpolicy(&at, pol);
	pc_check(r == 0, "setschedpolicy(%d) returned %s", pol, pc_errname(r));
	r = launch(NULL, &at, "sched", NULL, buf, sizeof buf, &st);
	if (r != 0)
		pc_unresolved("posix_spawn() with the new scheduler returned %s (privilege is implementation-defined)", pc_errname(r));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 127)
		pc_unresolved("the child could not take the scheduler (privilege is implementation-defined)");
	pc_check(has(buf, "[S:%d:", pol), "the child's policy is not %d (report \"%s\")", pol, buf);
	(void)posix_spawnattr_destroy(&at);
#else
	(void)at; (void)sp; (void)buf; (void)cur; (void)pol; (void)prio; (void)i; (void)r; (void)st; (void)pols;
	pc_unresolved("POSIX_SPAWN_SETSCHEDULER is not defined");
#endif
	pc_pass();
}
