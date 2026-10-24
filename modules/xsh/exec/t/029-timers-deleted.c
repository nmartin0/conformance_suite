/*
 * Per-process timers are deleted by exec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exec.029
 * VARIANT   exec.slow
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

extern char **environ;

static char *self;		/* this program: the new process image */
static int rfd = -1;		/* in the image: where its report goes */
static int report[2];
static pid_t kid;
static char fdarg[16];
static char *iv[32];

/* In the image: adds to the report. */
static void say(const char *fmt, ...)
{
	char b[700];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vsnprintf(b, sizeof b, fmt, ap);
	va_end(ap);
	if (n > 0)
		(void)write(rfd, b, (size_t)n < sizeof b ? (size_t)n : sizeof b - 1);
}

/* Forks a child with a report pipe; returns 1 in the child. */
static int spawn_child(void)
{
	if (pipe(report) != 0)
		pc_unresolved("pipe failed");
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed");
	if (kid == 0) {
		(void)close(report[0]);
		rfd = report[1];
		(void)snprintf(fdarg, sizeof fdarg, "%d", report[1]);
		return 1;
	}
	(void)close(report[1]);
	return 0;
}

/* In the child: arguments that run the image in mode m, then extra ones. */
static char **imgv(const char *m, ...)
{
	va_list ap;
	char *a;
	int n = 0;

	iv[n++] = self;
	iv[n++] = "img";
	iv[n++] = (char *)m;
	iv[n++] = fdarg;
	va_start(ap, m);
	while ((a = va_arg(ap, char *)) != NULL && n < 31)
		iv[n++] = a;
	va_end(ap);
	iv[n] = NULL;
	return iv;
}

/* In the child, when an exec function returned. */
_Noreturn static void returned(const char *fn)
{
	int e = errno;

	say("!%s returned -1 (%s)", fn, pc_errname(e));
	_exit(98);
}

/*
 * In the parent: reads the image's report to end of file and reaps the
 * child. A failed exec, or an image that did not end normally, is FAIL.
 */
static void collect(char *buf, size_t size)
{
	size_t n = 0;
	ssize_t r;
	int st;

	while (n < size - 1 && (r = read(report[0], buf + n, size - 1 - n)) > 0)
		n += (size_t)r;
	buf[n] = '\0';
	(void)close(report[0]);
	if (waitpid(kid, &st, 0) != kid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	if (buf[0] == '!')
		pc_fail("%s", buf + 1);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0,
	    "the new image ended with status %#x (report \"%s\")", st, buf);
}

static void expect_ok(const char *buf)
{
	pc_check(strcmp(buf, "ok") == 0, "the new image reports: %s", buf);
}

/* In the child, after an exec that should fail returned r. */
_Noreturn static void failed(long r)
{
	int e = errno;

	if (r != -1)
		say("R:%ld", r);
	else
		say("E:%s", pc_errname(e));
	_exit(98);
}

/*
 * In the parent: the outcome of an exec that should fail in the child,
 * with errno err (or alt, when another condition also applies). The
 * image, if the exec wrongly succeeds, reports "ran".
 */
static void expect_fail(const char *what, int err, int alt)
{
	char buf[256];
	size_t n = 0;
	ssize_t r;
	int st;

	while (n < sizeof buf - 1 && (r = read(report[0], buf + n, sizeof buf - 1 - n)) > 0)
		n += (size_t)r;
	buf[n] = '\0';
	(void)close(report[0]);
	if (waitpid(kid, &st, 0) != kid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	if (strcmp(buf, "ran") == 0)
		pc_fail("%s succeeded: the new image ran", what);
	if (strncmp(buf, "R:", 2) == 0)
		pc_fail("%s returned %s, expected -1", what, buf + 2);
	pc_check(strncmp(buf, "E:", 2) == 0, "%s: no report from the child (status %#x)", what, st);
	if (strcmp(buf + 2, pc_errname(err)) == 0 || (alt != 0 && strcmp(buf + 2, pc_errname(alt)) == 0))
		return;
	if (alt != 0)
		pc_fail("%s failed with %s, expected %s or %s", what, buf + 2, pc_errname(err), pc_errname(alt));
	pc_fail("%s failed with %s, expected %s", what, buf + 2, pc_errname(err));
}

/* Copies this program to path with the given mode. */
static void copy_self(const char *path, mode_t mode)
{
	char b[8192];
	int in = open(self, O_RDONLY), out;
	ssize_t r;

	if (in == -1)
		pc_unresolved("cannot open %s", self);
	out = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0700);
	if (out == -1)
		pc_unresolved("cannot create %s", path);
	while ((r = read(in, b, sizeof b)) > 0)
		if (write(out, b, (size_t)r) != r)
			pc_unresolved("short write to %s", path);
	(void)close(in);
	if (close(out) != 0 || chmod(path, mode) != 0)
		pc_unresolved("cannot finish %s", path);
}

static long long now_ns(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static int usr1_pending(void)
{
	sigset_t p;

	return sigpending(&p) == 0 && sigismember(&p, SIGUSR1) == 1;
}

/*
 * In the image: a[0] is when the old image armed its timer (CLOCK_MONOTONIC,
 * ns), a[1] the timer's length in ms. SIGUSR1 is blocked (the mask is
 * inherited), so a timer that fires shows as pending instead of ending us.
 */
static void image(const char *mode, int n, char **a)
{
	long long t0, end;
	long ms;

	(void)mode;
	if (n < 2) {
		say("missing arguments");
		return;
	}
	t0 = atoll(a[0]);
	ms = atol(a[1]);
	if (usr1_pending()) {
		/*
		 * Pending already: the exec took longer than the timer, which
		 * then expired in the old image, before exec deleted it.
		 */
		say("late %ld", (long)((now_ns() - t0) / 1000000));
		return;
	}
	end = t0 + (ms + 1000) * 1000000LL;	/* a second past the expiry */
	while (now_ns() < end) {
		long long left = end - now_ns();
		struct timespec ts;
		ts.tv_sec = (time_t)(left / 1000000000LL);
		ts.tv_nsec = (long)(left % 1000000000LL);
		(void)nanosleep(&ts, NULL);
	}
	say(usr1_pending() ? "fired" : "ok");
}

int main(int argc, char **argv)
{
	long ms = 2000;
	int attempt;

	if (argc >= 4 && strcmp(argv[1], "img") == 0) {
		rfd = atoi(argv[3]);
		self = argv[0];
		image(argv[2], argc - 4, argv + 4);
		return 0;
	}
	pc_begin("XSH.exec.029");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	self = argv[0];
	pc_test();
	for (attempt = 0; attempt < 3; attempt++) {
		char buf[256];
		if (spawn_child()) {
			static char t0[32], len[32];
			timer_t tid;
			struct sigevent ev;
			struct itimerspec it;
			sigset_t s;
			sigemptyset(&s);
			sigaddset(&s, SIGUSR1);
			memset(&ev, 0, sizeof ev);
			ev.sigev_notify = SIGEV_SIGNAL;
			ev.sigev_signo = SIGUSR1;
			memset(&it, 0, sizeof it);
			it.it_value.tv_sec = ms / 1000;
			it.it_value.tv_nsec = (ms % 1000) * 1000000L;
			if (sigprocmask(SIG_BLOCK, &s, NULL) != 0 || timer_create(CLOCK_REALTIME, &ev, &tid) != 0)
				_exit(99);
			(void)snprintf(t0, sizeof t0, "%lld", now_ns());
			(void)snprintf(len, sizeof len, "%ld", ms);
			if (timer_settime(tid, 0, &it, NULL) != 0)
				_exit(99);
			execv(self, imgv("timer", t0, len, (char *)NULL));
			returned("execv");
		}
		collect(buf, sizeof buf);
		if (strncmp(buf, "late ", 5) == 0) {
			/* No bound on how long an exec takes: try a longer timer. */
			ms = 3 * atol(buf + 5) + 1000;
			continue;
		}
		pc_check(strcmp(buf, "fired") != 0, "the old image's timer (%ld ms) fired in the new image", ms);
		expect_ok(buf);
		pc_pass();
	}
	pc_unresolved("each exec took longer than the timer (last %ld ms), which so expired before exec", ms);
}
